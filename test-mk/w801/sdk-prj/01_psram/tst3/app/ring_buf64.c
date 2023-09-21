#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"

#include "wm_cpu.h"
#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_io.h"
#include "wm_mem.h"
#include "wm_osal.h"
#include "wm_pmu.h"
#include "wm_psram.h"
#include "wm_regs.h"
#include "wm_rtc.h"
#include "wm_type_def.h"

#include <FreeRTOS.h>
#include <task.h>

#include "ring_buf64.h"
#include "psram.h"

static volatile int iBUF_CNT_CHUNK=0;                                                              
// 8,16,32,64,128,256,512,1024 ... размер буфера обязательно должен быть
// равен степени двойки! 4096 = 262144 byte
static volatile int iBUF_MASK=0; //(BUF_SIZE - 1)

  /* 256 x 32 = 8192, буффер, работает без сбоев, странно... */
  /* 64 x 32 = 2048, буффер, работает без сбоев, странно... */
  /* 64 x 64 = 4096, буффер, работает без сбоев, странно... */
  /* 128 x 64 = 8192, сбоит! что то аппаратное наверно */


//static volatile bool b_psram = false;
static volatile int idxIN = 0;
static volatile int idxOUT = 0;
static volatile int idxUSE = 0;
static u8 *ring_buf = NULL; // [BUF_SIZE*BUF_BYTE_CHUNK_SIZE]; //  = NULL; //

u8 *
HeadRingBuf (void)
{
  return ring_buf;
}
int RingBufChunkTotal (void)
{
  return iBUF_CNT_CHUNK;
}

u8 *
CreateRingBuf (void)
{
  //taskENTER_CRITICAL (); - убрал, потому что с этим время от времени, попадает в exception CPU 3
  if (ring_buf == NULL)
    {
      //b_psram = false;

      wm_psram_config (1);
      //      tls_io_cfg_set(WM_IO_PA_15, WM_IO_OPTION1);/*CK*/
      //      tls_io_cfg_set(WM_IO_PB_27, WM_IO_OPTION1);/*CS*/
      //      tls_io_cfg_set(WM_IO_PB_02, WM_IO_OPTION4);/*D0*/
      //      tls_io_cfg_set(WM_IO_PB_03, WM_IO_OPTION4);/*D1*/
      //      tls_io_cfg_set(WM_IO_PB_04, WM_IO_OPTION4);/*D2*/
      //      tls_io_cfg_set(WM_IO_PB_05, WM_IO_OPTION4);/*D3*/
      //      tls_open_peripheral_clock(TLS_PERIPHERAL_TYPE_PSRAM);            


      //чтоб там инициализация DMA была, мало ли пригодится
      //psram_init (
      //  PSRAM_SPI ); // PSRAM_QPI - почему то не заработал, что то не так сделал
      //d_psram_init (PSRAM_SPI,2,0,0,0);
      d_psram_init (PSRAM_SPI,2,2,1,2);
      tls_os_time_delay (HZ/10);
      if(d_psram_check())
        {
          iBUF_CNT_CHUNK=4096;//262144;
          iBUF_MASK=(iBUF_CNT_CHUNK - 1);
          ring_buf = dram_heap_malloc (BUF_BYTE_CHUNK_SIZE * iBUF_CNT_CHUNK);
          //b_psram = true; - убрал, потому что с этим время от времени memcpy_dma - портит буффер
        }
    }
  if (ring_buf == NULL)
    {
      iBUF_CNT_CHUNK= 16;//2048;
      iBUF_MASK=(iBUF_CNT_CHUNK - 1);
      ring_buf = tls_mem_alloc (BUF_BYTE_CHUNK_SIZE * iBUF_CNT_CHUNK);
      printf (" CreateRingBuf address= %d, size %d \n\n",
              (u32)ring_buf, BUF_BYTE_CHUNK_SIZE * iBUF_CNT_CHUNK);
    }
  else
    {
      printf (" use RingBuf address= %d, size %d \n\n", (unsigned int)ring_buf
           ,BUF_BYTE_CHUNK_SIZE *iBUF_CNT_CHUNK);
    }
  idxIN = 0;
  idxOUT = 0;
  idxUSE = 0;
  //taskEXIT_CRITICAL ();
  return ring_buf;
}

u8
PushRingBufChunk (u8 *Buffer)
{
  u8 u8_put = 0;
  //taskENTER_CRITICAL ();
  if (ring_buf != NULL && idxUSE < iBUF_CNT_CHUNK)
    {
      u8 *bb_put = ring_buf + (idxIN * BUF_BYTE_CHUNK_SIZE);
      //При помещении значения value в буфер используется индекс idxIN. Это
      //делается так:
      //if (b_psram)
      //   {
      //     while(memcpy_dma (bb_put, Buffer, BUF_BYTE_CHUNK_SIZE)!=BUF_BYTE_CHUNK_SIZE);
      //   }
      //else
      //   memcpy (bb_put, Buffer, BUF_BYTE_CHUNK_SIZE);

      int offset = 0;
      int dw_length = (BUF_BYTE_CHUNK_SIZE&(~0x03))>>2;
      while(dw_length--)
	{
		M32((bb_put+offset)) = M32((Buffer+offset));
		offset+=4;
	}

       
      // printf (" PushRingBuf address= %d, idxIN = %d,
      // idxUSE=%d\n\n",(int)bb_put,idxIN,idxUSE);
      idxIN++;
      idxIN &= iBUF_MASK;
      idxUSE++;
      u8_put = BUF_BYTE_CHUNK_SIZE;
    }
  //taskEXIT_CRITICAL ();
  return u8_put;
}

u8
PopRingBufChunk (u8 *Buffer)
{
  u8 u8_get = 0;
  //taskENTER_CRITICAL ();
  if (ring_buf != NULL && idxUSE > 0)
    {
      u8 *bb_get = ring_buf + (idxOUT * BUF_BYTE_CHUNK_SIZE);
      //Операция выборки из буфера происходит похожим образом, только
      //используется индекс idxOUT:
      //if (b_psram)
      //   {
      //     while(memcpy_dma (Buffer, bb_get, BUF_BYTE_CHUNK_SIZE)!=BUF_BYTE_CHUNK_SIZE);
      //   }
      //else
      //   memcpy (Buffer, bb_get, BUF_BYTE_CHUNK_SIZE);

      int offset = 0;
      int dw_length = (BUF_BYTE_CHUNK_SIZE&(~0x03))>>2;
      while(dw_length--)
	{
		M32((Buffer+offset)) = M32((bb_get+offset));
		offset+=4;
	}


      // printf (" PopRingBuf address= %d, idxOUT = %d, idxUSE=%d
      // \n\n",(int)bb_get,idxOUT,idxUSE);
      idxOUT++;
      idxOUT &= iBUF_MASK;
      idxUSE--;
      u8_get = BUF_BYTE_CHUNK_SIZE;
    }
  //taskEXIT_CRITICAL ();
  return u8_get;
}

void
ResetRingBuf (void)
{
  //taskENTER_CRITICAL ();
  //Сбросить данные буфера (т. е. удалить их оттуда) тоже очень просто - для
  //этого в idxOUT записывают значение idxIN: idxOUT = idxIN;
  idxIN = 0;
  idxOUT = 0;
  idxUSE = 0;
  printf (" ResetRingBuf address= %d, size %d \n\n", (unsigned int)ring_buf,BUF_BYTE_CHUNK_SIZE *iBUF_CNT_CHUNK);
  //taskEXIT_CRITICAL ();
  // printf (" ResetRingBuf \n\n");
}

//сколько данных в буфере.
// static u8
// idxDiff ()
//{
//  return (idxIN - idxOUT) & iBUF_MASK;
//
//    if (idxIN >= idxOUT)
//        return (idxIN - idxOUT);
//    else
//        return ((BUF_SIZE - idxOUT) + idxIN);
//
//}

int
RingBufChunkAvailable (void)
{
  int i_ChunkFree = 0;
  // printf (" RingBufSpacesAvailable idxIN = %d idxOUT = %d = %d\n\n",idxIN ,
  // idxOUT, ((idxIN - idxOUT) & iBUF_MASK));
  //  return BUF_SIZE - ((idxIN - idxOUT) & iBUF_MASK) - 1;
  // if((BUF_SIZE - idxUSE - 4)<=0) return 0;
  //taskENTER_CRITICAL ();
  i_ChunkFree = (iBUF_CNT_CHUNK - idxUSE); // * BUF_BYTE_CHUNK_SIZE;
  //taskEXIT_CRITICAL ();
  return i_ChunkFree;
}
