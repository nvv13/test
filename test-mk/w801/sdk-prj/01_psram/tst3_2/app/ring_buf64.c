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
//#include <message_buffer.h>
#include "ringbuffer.h"

#include "ring_buf64.h"
#include "psram.h"


static uint8_t * ucStorageBuffer = NULL; // [ xMessageBufferSize+1 ];

/* The variable used to hold the ringbuffer buffer structure. */
static ringbuffer_t my_ring_buf_Struct;

static volatile int iBUF_CNT_CHUNK=0;                                                              
// 8,16,32,64,128,256,512,1024 ... размер буфера обязательно должен быть
// равен степени двойки! 4096 = 262144 byte

  /* 256 x 32 = 8192, буффер, работает без сбоев, странно... */
  /* 64 x 32 = 2048, буффер, работает без сбоев, странно... */
  /* 64 x 64 = 4096, буффер, работает без сбоев, странно... */
  /* 128 x 64 = 8192, сбоит! что то аппаратное наверно */

u8 *
HeadRingBuf (void)
{
  return ucStorageBuffer;
}
int RingBufChunkTotal (void)
{
  return iBUF_CNT_CHUNK;
}

u8 *
CreateRingBuf (void)
{
  taskENTER_CRITICAL ();
  if (ucStorageBuffer == NULL)
    {
      wm_psram_config (1);
      //      tls_io_cfg_set(WM_IO_PA_15, WM_IO_OPTION1);/*CK*/
      //      tls_io_cfg_set(WM_IO_PB_27, WM_IO_OPTION1);/*CS*/
      //      tls_io_cfg_set(WM_IO_PB_02, WM_IO_OPTION4);/*D0*/
      //      tls_io_cfg_set(WM_IO_PB_03, WM_IO_OPTION4);/*D1*/
      //      tls_io_cfg_set(WM_IO_PB_04, WM_IO_OPTION4);/*D2*/
      //      tls_io_cfg_set(WM_IO_PB_05, WM_IO_OPTION4);/*D3*/
      //      tls_open_peripheral_clock(TLS_PERIPHERAL_TYPE_PSRAM);            

      //d_psram_init (PSRAM_SPI,2,0,0,0);
      d_psram_init (PSRAM_SPI,2,2,1,2);
      tls_os_time_delay (HZ/10);
      if(d_psram_check())
        {
          iBUF_CNT_CHUNK=4096;//262144;
          ucStorageBuffer = dram_heap_malloc ((BUF_BYTE_CHUNK_SIZE) * iBUF_CNT_CHUNK +1);
        }
    }
  if (ucStorageBuffer == NULL)
    {
      iBUF_CNT_CHUNK= 32;//2048;
      ucStorageBuffer = (u8 *)tls_mem_alloc ((BUF_BYTE_CHUNK_SIZE) * iBUF_CNT_CHUNK +1);
      printf (" CreateRingBuf address= %d, size %d \n\n",
              (u32)ucStorageBuffer, (BUF_BYTE_CHUNK_SIZE) * iBUF_CNT_CHUNK +1);
    }
  else
    {
      printf (" use ucStorageBuffer address= %d, size %d \n\n", (unsigned int)ucStorageBuffer
           ,(BUF_BYTE_CHUNK_SIZE) * iBUF_CNT_CHUNK +1);
    }
  if (ucStorageBuffer != NULL)
    {
    ringbuffer_init(&my_ring_buf_Struct, (char*)ucStorageBuffer,  (BUF_BYTE_CHUNK_SIZE) * iBUF_CNT_CHUNK);
    }
  taskEXIT_CRITICAL ();
  return ucStorageBuffer;
}

u8
PushRingBufChunk (u8 *Buffer)
{
  u8 u8_put = 0;
  taskENTER_CRITICAL ();
  if (ucStorageBuffer != NULL &&  ringbuffer_get_free(&my_ring_buf_Struct) >= (BUF_BYTE_CHUNK_SIZE))
    {
    u8_put = ringbuffer_add(&my_ring_buf_Struct, (char*)Buffer, BUF_BYTE_CHUNK_SIZE);
    //u8_put = BUF_BYTE_CHUNK_SIZE;
    }
  taskEXIT_CRITICAL ();
  return u8_put;
}

u8
PopRingBufChunk (u8 *Buffer)
{
  u8 u8_get = 0;
  taskENTER_CRITICAL ();
  if (ucStorageBuffer != NULL)
    {
    u8_get = ringbuffer_get(&my_ring_buf_Struct, (char*)Buffer, BUF_BYTE_CHUNK_SIZE);
    }
  taskEXIT_CRITICAL ();
  return u8_get;
}

void
ResetRingBuf (void)
{
  taskENTER_CRITICAL ();
  //Сбросить данные буфера (т. е. удалить их оттуда) тоже очень просто - для
  ringbuffer_init(&my_ring_buf_Struct, (char*)ucStorageBuffer,  (BUF_BYTE_CHUNK_SIZE) * iBUF_CNT_CHUNK);
  taskEXIT_CRITICAL ();
  printf (" ResetRingBuf address= %d, size %d \n\n", (unsigned int)ucStorageBuffer,(BUF_BYTE_CHUNK_SIZE) * iBUF_CNT_CHUNK +1);
}


int
RingBufChunkAvailable (void)
{
  int i_get = 0;
  taskENTER_CRITICAL ();
  i_get = ringbuffer_get_free(&my_ring_buf_Struct) / (BUF_BYTE_CHUNK_SIZE);
  taskEXIT_CRITICAL ();
  return i_get;
}
