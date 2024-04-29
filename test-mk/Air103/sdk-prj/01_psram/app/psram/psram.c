/*

Взято из
https://gitee.com/beewu123/w80x-sdk-lcd.git
там Apache License  Version 2.0, January 2004
*/

#include <stdio.h>

#include "wm_type_def.h"

#include "psram.h"
#include "wm_cpu.h"

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_io.h"
#include "wm_osal.h"
#include "wm_psram.h"
#include "wm_regs.h"


/* Heap should align to DRAM_HEAP_ALIGNMENT */
#define DRAM_HEAP_BASE (0x30000000)
/* Size should be smaller than ~DRAM_HEAP_MAGIC */
//#define DRAM_HEAP_SIZE              (2*1024 * 1024)
#define DRAM_HEAP_SIZE                                                        \
  (8 * 1024 * 1024) // у нас LY68L6400 памяти 64M Bits, или 8 MB

#define ALIGN(x, a) (((x) + (a)-1) & ~((a)-1))
#define DRAM_HEAP_ALIGNMENT 4

/* At the beginning of allocated memory */
#define DRAM_HEAP_BLK_HEAD_SIZE                                               \
  ALIGN (sizeof (ih_blk_head_t), DRAM_HEAP_ALIGNMENT)
/* Block sizes must not get too small. */
#define DRAM_HEAP_BLK_MIN ((DRAM_HEAP_BLK_HEAD_SIZE) << 1)

/* blk belong to APP, magic set
   blk belong to free list, magic clear */
#define DRAM_HEAP_MAGIC (0xF0000000)

/* block head, before each blocks */
typedef struct ih_blklist
{
  /* free memory block list, order by address*/
  struct ih_blklist *next;
  /* when blk in APP: DRAM_HEAP_MAGIC & size
     when blk in freelist: only size
     size include ih_blk_head_t self */
  size_t magic_size;
} ih_blk_head_t;

/* free block list: order by address, from low to high */
/* first free block. */
static ih_blk_head_t s_freelist_head;
/* point to last free block. */
static ih_blk_head_t *s_freelist_tail;

/* For statistic. */
static size_t s_heap_free_size = DRAM_HEAP_SIZE;
static size_t s_heap_free_size_min = DRAM_HEAP_SIZE;

/* Init heap, with one free block */
static void
dram_heap_init (void)
{
  ih_blk_head_t *free_blk;

  if (DRAM_HEAP_BASE % DRAM_HEAP_ALIGNMENT != 0
      || DRAM_HEAP_SIZE > (~DRAM_HEAP_MAGIC)
      || DRAM_HEAP_SIZE < DRAM_HEAP_BLK_MIN * 2)
    {
      printf ("[dram_heap] fatal error: heap parameter invalid!\n");
      return;
    }

  s_freelist_head.next = (void *)DRAM_HEAP_BASE;
  s_freelist_head.magic_size = (size_t)0;

  s_freelist_tail
      = (void *)(DRAM_HEAP_BASE + DRAM_HEAP_SIZE - DRAM_HEAP_BLK_HEAD_SIZE);
  s_freelist_tail->next = NULL;
  s_freelist_tail->magic_size = 0;

  /* Only one block when init. */
  free_blk = (void *)DRAM_HEAP_BASE;
  free_blk->next = s_freelist_tail;
  free_blk->magic_size = (size_t)s_freelist_tail - (size_t)free_blk;

  s_heap_free_size = free_blk->magic_size;
  s_heap_free_size_min = free_blk->magic_size;
}

static void
dram_heap_freeblk_insert (ih_blk_head_t *blk_insert)
{
  ih_blk_head_t *blk_before; /* before the blk_insert */
  ih_blk_head_t *blk_after;  /* after  the blk_insert */

  /* freelist is ordered by address, find blk_before */
  for (blk_before = &s_freelist_head; blk_before->next < blk_insert;
       blk_before = blk_before->next)
    {
      if (blk_before == s_freelist_tail)
        {
          return;
        }
    }
  blk_after = blk_before->next;

  /* now: blk_before < blk_insert < blk_after */

  /* try to merge blk_before and blk_insert */
  if ((char *)blk_before + blk_before->magic_size == (char *)blk_insert)
    {
      blk_before->magic_size += blk_insert->magic_size;
      blk_insert = blk_before;
    }
  else
    {
      /* do not merge, just insert new node to freelist */
      blk_before->next = blk_insert;
    }

  /* try to merge blk_insert and blk_after */
  if (blk_after != s_freelist_tail
      && (char *)blk_insert + blk_insert->magic_size == (char *)blk_after)
    {
      blk_insert->magic_size += blk_after->magic_size;
      blk_insert->next = blk_after->next;
    }
  else
    {
      /* do not merge, just insert new node to freelist */
      blk_insert->next = blk_after;
    }
}

void *
dram_heap_malloc (size_t alloc_size)
{
  ih_blk_head_t *blk_alloc;
  ih_blk_head_t *blk_prev;
  ih_blk_head_t *blk_left;

  // krhino_sched_disable();

  /* first call to malloc, init is needed */
  if (s_freelist_tail == NULL)
    {
      printf ("heap init\r\n");
      dram_heap_init ();
    }

  if ((alloc_size == 0) || (alloc_size > s_heap_free_size))
    {
      /* not enough memory */
      // krhino_sched_enable();
      return NULL;
    }

  alloc_size += DRAM_HEAP_BLK_HEAD_SIZE;
  alloc_size = ALIGN (alloc_size, DRAM_HEAP_ALIGNMENT);

  /* find a free block bigger than alloc_size */
  blk_prev = &s_freelist_head;
  blk_alloc = s_freelist_head.next;
  while ((blk_alloc->magic_size < alloc_size) && (blk_alloc->next != NULL))
    {
      blk_prev = blk_alloc;
      blk_alloc = blk_alloc->next;
    }
  if (blk_alloc->next == NULL)
    {
      /* this means "blk_alloc == s_freelist_tail" */
      // krhino_sched_enable();
      return NULL;
    }

  /* delete blk_alloc from freelist */
  blk_prev->next = blk_alloc->next;

  /* check whether blk_alloc can split */
  if (blk_alloc->magic_size - alloc_size > DRAM_HEAP_BLK_MIN)
    {
      /* split */
      blk_left = (void *)((char *)blk_alloc + alloc_size);
      blk_left->magic_size = blk_alloc->magic_size - alloc_size;

      blk_alloc->magic_size = alloc_size;

      /* Insert the new block into the list of free blocks. */
      dram_heap_freeblk_insert (blk_left);
    }

  /* update statistic */
  s_heap_free_size -= blk_alloc->magic_size;
  if (s_heap_free_size < s_heap_free_size_min)
    {
      s_heap_free_size_min = s_heap_free_size;
    }

  // krhino_sched_enable();

  /* blk belong to APP, magic set */
  blk_alloc->magic_size |= DRAM_HEAP_MAGIC;
  blk_alloc->next = NULL;

  /* app used addr is after ih_blk_head_t */
  return (void *)((char *)blk_alloc + DRAM_HEAP_BLK_HEAD_SIZE);
}

void
dram_heap_free (void *pfree)
{
  ih_blk_head_t *free_blk;

  if (pfree == NULL)
    {
      return;
    }

  /* app used addr is after ih_blk_head_t */
  free_blk = (ih_blk_head_t *)((char *)pfree - DRAM_HEAP_BLK_HEAD_SIZE);

  /* blk damaged check */
  if (free_blk->next != NULL
      || (free_blk->magic_size & DRAM_HEAP_MAGIC) != DRAM_HEAP_MAGIC)
    {
      printf ("[dram_heap] fatal error: block has been destroyed!\n");
      return;
    }

  /* blk belong to free list, magic clear */
  free_blk->magic_size &= ~DRAM_HEAP_MAGIC;
  /* update statistic */
  s_heap_free_size += free_blk->magic_size;

  // krhino_sched_disable();

  dram_heap_freeblk_insert (free_blk);

  // krhino_sched_enable();
}

size_t
dram_heap_free_size (void)
{
  return s_heap_free_size;
}

size_t
dram_heap_free_size_min (void)
{
  return s_heap_free_size_min;
}

/* if addr is in dram_heap, return 1
   else return 0*/
int
dram_heap_check_addr (void *addr)
{
  if ((size_t)addr - DRAM_HEAP_BASE < DRAM_HEAP_SIZE)
    {
      return 1;
    }
  else
    {
      return 0;
    }
}




/*

4.4.3 PSRAM
W800 has a built-in PSRAM controller with SPI/QSPI interface,
 supports external PSRAM device access with a maximum capacity of 64Mb,
 and provides bus mode of PSRAM read, write and erase operations. 
The maximum read and write speed is 80MHz. 
When the storage capacity needs to be expanded, the off-chip PSRAM can be used to expand
Fill code storage space or data storage space. 
PSRAM also supports XIP execution of programs, and CPU Cache also supports cache
data in PSRAM.



25 PSRAM interface controller

25.1 Function overview
W800 has a built-in PSRAM controller with SPI/QSPI interface,
 supports external PSRAM device access, and provides bus-based PSRAM reading
Write and erase operations. The maximum read and write speed is 80MHz.

25.2 Main Features
= Support read and write access to external PSRAM
= Configurable as SPI and QSPI
= SPI/QSPI clock frequency can be configured
= Support BURST INC mode access
= Support half sleep mode of PSRAM

25.3 Functional Description
The full name of PSRAM is Pseudo static random access memory,
 which refers to pseudo-static random access memory. 
Compared with traditional SRAM
It has the advantages of small package, large capacity and low cost,
 and is mainly used for data caching in IoT applications. 
The interface is mostly SPI, QSPI . 
The interface pins mainly include clock signal SCK, chip select signal CS and 4 bidirectional data IOs. 
The PSRAM controller provided by the W800 can
It is accessed by the bus mode of PSRAM supporting SPI/QSPI interface,
 the maximum working clock rate is 80MHz, and the maximum capacity supports 64Mb.

25.3.1 Pin Description
SCLK: SPI interface clock, the SCLK cycle is set by PSRAM_CTRL[7:4],
 the minimum frequency division value that can be set is 3,
 and the default is AHB Divide by 4 of the clock.
CS: SPI interface chip select signal
SIO0: SPI mode data input, QSPI mode SD[0]
SIO1: SPI mode data output, QSPI mode SD[1]
SIO2: QSPI mode SD[2]
SIO3: QSPI mode SD[3]

25.3.2 Access Mode Settings
The PSRAM controller supports two access modes:
 SPI and four-wire QSPI to the external PSRAM, and the default is SPI. by setting
  PSRAM_CTRL[1] configures the mode of the SPI.
  PSRAM_CTRL[1] is 0 by default, that is, SPI mode.
 At this time, it takes 64 SCLK cycles to complete a write operation and
 64 SCLK cycles to complete a read operation. 72 SCLK cycles are required.

If PSRAM works in SPI mode,
 when writing 1 to PSRAM_CTRL[1], the controller will send command 35H to PSRAM,
When PSRAM_CTRL[1] is read as 1,
 it means that the command is sent and the PSRAM enters the QPI mode.
16 SCLK cycles, 22 SCLK cycles are required to complete a read operation.

If PSRAM works in QPI mode, when writing 0 to PSRAM_CTRL[1],
 the controller will send command F5H to PSRAM,
When PSRAM_CTRL[1] is read as 0, it indicates that the command transmission is completed,
 and the PSRAM enters the SPI mode.

The PSRAM working mode must be set after the initialization operation is completed,
 but cannot be set at the same time.

25.3.3 PSRAM initialization
 Before the first use, after the PSRAM is powered on and stabilized,
 write 1 to the register PSRAM_CTRL[0] to start the PSRAM reset initialization
 operation, that is, send 66H and 99H commands to PSRAM. 
By default, SPI mode is used to send, that is, 8 SCLK + tCPH + 8 time of SCLK. 
After initialization, the hardware automatically clears PSRAM_CTRL[0].
The initialization operation will restore the PSRAM to SPI mode.

The recommended initialization process is:
(1) Write PSRAM_CTRL[0] to 1
(2) Wait until PSRAM_CTRL[0] is automatically cleared
(3) Reset the PSRAM controller by soft reset
(4) Reset other required parameters of PSRAM_CTRL

25.3.4 Access Methods of PSRAM
The way of reading and writing to PSRAM is the same as that of ordinary SRAM,
 that is, writing/reading data to the corresponding bus address.

25.3.5 BURST function
By setting PSRAM_CTRL[2], the controller can support the BURST initiated by the AHB bus,
 that is, when the HBURST on the AHB bus is
At 1/3/5/7, it means that the AHB bus starts a continuous read/write with an address increment.
At this time, in order to improve the access speed of PSRAM, control the
After completing the read/write of a word, the processor will not pull CS high,
 but directly read/write the data of the next word.
The OVERTIMER register is used to set the maximum time for CS to be low,
 the unit is the number of cycles of HCLK, each time a BURST operation starts,
The internal counter starts counting from 0.
When the counter value is greater than the set value,
 the controller stops automatically after completing the read/write of the current word.
BURST operation, directly change CS to high level,
 if there is still data on the AHB bus to read/write at this time,
 it will be treated as a separate WORD
conduct.
The PSRAM controller does not support BURST in the form of WRAP. 
If WRAP BURST is used to access PSRAM, please
PSRAM_CTRL[2] is set to 0.

25.4 Register Description
25.4.1 Register List

                  Table 205 PSRAM Controller Register List
------------------------------------------------------------------------------------------
offset address| name                   |abbreviation|describe                 |Defaults
------------------------------------------------------------------------------------------
0X0000_0000   |Control Register        |PSRAM_CTRL  |PSRAM Controller Settings|0X0000_0000
0X0000_0004   |Timeout Control Register|OVERTIMER   |CS Timeout Control       |0X0000_0000
------------------------------------------------------------------------------------------

25.4.2 Command Information Register

                  Table 206 PSRAM Control Setting Register
----------------------------------------------------------------------------------
bit    |access|Instructions                                                  |reset value
----------------------------------------------------------------------------------
[31:12]|RO    |RSV                                                           |'b0
----------------------------------------------------------------------------------
[11]   |RW    |HSM, Halfsleep mode enabled                                   |1'b0
       |      |1: Enable PSRAM half-sleep mode                               |
       |      |0: Clear half sleep mode                                      |
----------------------------------------------------------------------------------
[10:8] |RW    |tCPH, the shortest time setting of CS high level,             |3'd6
       |      |the unit is the number of AHB clock cycles,                   |
       |      |must be greater than 1, the specific time depends on the      |
       |      |                                                              |
       |      |Set it according to the instructions in the same psram manual,|
       |      |if you don't know, you can not modify the default value.      |
----------------------------------------------------------------------------------
[7:4]  |RW    |SPI frequency divider setting                                 |4'd4
       |      |                                                              |
       |      |It can only be configured to a value of 2 or more,            |
       |      |and the written value is a multiple of the frequency division |
----------------------------------------------------------------------------------
[3]    |RO    |RSV                                                           |
----------------------------------------------------------------------------------
[2]    |RW    |INC_EN BURST function enable                                  |1'b0
       |      |1: Support BURST function on AHB bus                          |
       |      |0: BURST function is not supported                            |
----------------------------------------------------------------------------------
[1]    |RW    |QUAD                                                          |1'b0
       |      |Write 1 to enable QPI mode of PSRAM,write 0 to enable SPI mode|
       |      |Read this flag to know which mode the current PSRAM is in.    |
----------------------------------------------------------------------------------
[0]    |RW    |PSRAM reset                                                   |1'b0
       |      |Write 1 to start the reset operation to PSRAM, and it will be |
       |      | automatically cleared after reset.                           |
----------------------------------------------------------------------------------

25.4.3 Timeout Control Register

                   Table 207 CS Timeout Control Register
----------------------------------------------------------------------------------
bit    |access|Instructions                                                  |reset value
----------------------------------------------------------------------------------
[11:0] |RW    |Timeout register setting,                                     |12'd0
       |      |  sets the maximum time for CS to be low, for BURST mode      |
----------------------------------------------------------------------------------

*/

void d_psram_init(psram_mode_t mode, u8 frequency_divider,u8 tCPH, u8 BURST, u16 OVERTIMER)
{

        if (s_freelist_tail != NULL)
          {
            printf("s_freelist_tail != NULL, PSRAM no init, skip\n");
            return;
          }

	volatile unsigned int value = 0x00000000;

        value=(OVERTIMER & 0x7FF);
        printf("set value to HR_PSRAM_OVERTIMER_ADDR = 0x%08X\n", value);
        tls_reg_write32(HR_PSRAM_OVERTIMER_ADDR, value);

        value = 0x00000000;

        // [10:8] |RW    |tCPH, the shortest time setting of CS high level,             |3'd6
        tCPH &= 0x7; 
        if(tCPH<0x2)
            {
                tCPH=0x6;//default
            }
        value |= tCPH<<8; 

        // [7:4]  |RW    |SPI frequency divider setting                                 |4'd4
        // example PSRAM clock = 240MHZ / 3 = 80MHz 
        frequency_divider &= 0xF;
        if(frequency_divider<0x2)
            {
                frequency_divider=0x4;//default
            }
        value |= frequency_divider<<4; 

 	if(BURST)
	{
		value |= 0x1<<2;
	}

 	if(mode == PSRAM_QPI)
	{
		value |= 0x1<<1;
	}

	/*reset psram*/
	value |= 0x1;
        printf("set value to HR_PSRAM_CTRL_ADDR = 0x%08X\n", value);

	tls_reg_write32(HR_PSRAM_CTRL_ADDR, value);
	do{
		value = tls_reg_read32(HR_PSRAM_CTRL_ADDR);
	}while(value&0x1);	

//	psram_channel = tls_dma_request(0, 0);
//    tls_dma_irq_register(psram_channel, psram_DMA_Channel0_IRQHandler, NULL, TLS_DMA_IRQ_TRANSFER_DONE);
}



// *************************************************************
// Definitions for three 32bit variables inside the PSRAM address range (from 0x30000000 to 0x30800000)
// *************************************************************
#define VAR1_PSRAM_ADDR 0x30000000
#define VAR2_PSRAM_ADDR 0x30400000
#define VAR3_PSRAM_ADDR 0x30000100

#define var1Psram (*(volatile u32 *)VAR1_PSRAM_ADDR)    // Used volatile here just to force a read to check if the PSRAM is really present
#define var2Psram (*(volatile u32 *)VAR2_PSRAM_ADDR)
#define var3Psram (*(volatile u32 *)VAR3_PSRAM_ADDR)

#define VALUE_1   0x000000AA
#define VALUE_2   0x0000BB11
#define VALUE_3   0xAABBCC00


#define HR_PSRAM_CTRL (*(volatile u32 *)HR_PSRAM_CTRL_ADDR)             
#define HR_PSRAM_OVERTIMER (*(volatile u32 *)HR_PSRAM_OVERTIMER_ADDR)   

/***************************************************************
* TIPC Registers definitions (not used here)
****************************************************************/
#define HR_TIPC_BASE          (HR_APB_BASE_ADDR + 0x2400)
#define IP_TRUST_VDL0_ADDR    (HR_TIPC_BASE)
#define IP_TRUST_VDL1_ADDR    (HR_TIPC_BASE + 0x4)
#define IP_TRUST_VLD0 (*(volatile u32 *)IP_TRUST_VDL0_ADDR)
#define IP_TRUST_VLD1 (*(volatile u32 *)IP_TRUST_VDL1_ADDR)


bool d_psram_check(void)
{
    bool b_ok=false;

    u32 var1=var1Psram;
    u32 var2=var2Psram;
    u32 var3=var3Psram;

    var1Psram = (u32) VALUE_1;
    var2Psram = (u32) VALUE_2;
    var3Psram = (u32) VALUE_3;

    printf("var1Psram = 0x%08X\n", var1Psram);
    printf("var2Psram = 0x%08X\n", var2Psram);
    printf("var3Psram = 0x%08X\n", var3Psram);

    if ((var1Psram == VALUE_1) && (var2Psram == VALUE_2) && (var3Psram == VALUE_3))
    {
      printf(" PSRAM found \n");
      printf("  Address of HR_PSRAM_CTRL = 0x%08X\n", HR_PSRAM_CTRL_ADDR);
      printf("  Content of HR_PSRAM_CTRL = 0x%08X\n", HR_PSRAM_CTRL);
      printf("  Content of HR_PSRAM_OVERTIMER = 0x%08X\n", HR_PSRAM_OVERTIMER);   
      printf("  Content of IP_TRUST_VLD0 = 0x%08X\n", IP_TRUST_VLD0);             
      printf("  Content of IP_TRUST_VLD1 = 0x%08X\n", IP_TRUST_VLD1);             
      b_ok=true;
    }
    else
    {
      printf(" PSRAM not found \n");
    }

    var1Psram=var1;
    var2Psram=var2;
    var3Psram=var3;

    return b_ok;
}


