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

// static bool b_psram = true;
static volatile int idxIN = 0;
static volatile int idxOUT = 0;
static volatile int idxUSE = 0;
static u8 *ring_buf = NULL; // [BUF_SIZE*BUF_BYTE_CHUNK_SIZE]; //  = NULL; //

u8 *
HeadRingBuf (void)
{
  return ring_buf;
}

u8 *
CreateRingBuf (void)
{

  taskENTER_CRITICAL ();
  //  if (b_psram)
  //    {
  //      wm_psram_config (0);
  //      psram_init (PSRAM_SPI); // PSRAM_QPI - почему то не заработал, что то
  //      не
  //                              // так сделал
  //      ring_buf = (u8 *)PSRAM_ADDR_START;
  //    }
  //  else
  //    {
  if (ring_buf == NULL)
    {
      ring_buf = tls_mem_alloc (BUF_SIZE * BUF_BYTE_CHUNK_SIZE);
      printf (" CreateRingBuf address= %d, size %d \n\n",
              (unsigned int)ring_buf, BUF_SIZE * BUF_BYTE_CHUNK_SIZE);
    }
  else
    {
      printf (" use RingBuf address= %d, size %d \n\n", (unsigned int)ring_buf,
              BUF_SIZE * BUF_BYTE_CHUNK_SIZE);
    }
  //    }
  idxIN = 0;
  idxOUT = 0;
  idxUSE = 0;
  taskEXIT_CRITICAL ();
  return ring_buf;
}

u8 *
PushRingBuf (void)
{
  u8 *bb_put = NULL;
  taskENTER_CRITICAL ();
  if (ring_buf != NULL && idxUSE < BUF_SIZE)
    {
      //При помещении значения value в буфер используется индекс idxIN. Это
      //делается так:
      //  if (b_psram)
      //    memcpy_dma (ring_buf + (idxIN * BUF_BYTE_CHUNK_SIZE), Buffer,
      //                BUF_BYTE_CHUNK_SIZE);
      //  else
      //    memcpy (ring_buf + (idxIN * BUF_BYTE_CHUNK_SIZE), Buffer,
      //            BUF_BYTE_CHUNK_SIZE);
      bb_put = ring_buf + (idxIN * BUF_BYTE_CHUNK_SIZE);
      // printf (" PushRingBuf address= %d, idxIN = %d,
      // idxUSE=%d\n\n",(int)bb_put,idxIN,idxUSE);
      idxIN++;
      idxIN &= BUF_MASK;
      idxUSE++;
    }
  taskEXIT_CRITICAL ();
  return bb_put;
}

u8 *
PopRingBuf (void)
{
  u8 *bb_get = NULL;
  taskENTER_CRITICAL ();
  if (ring_buf != NULL && idxUSE > 1)
    {
      //Операция выборки из буфера происходит похожим образом, только
      //используется индекс idxOUT:
      //  if (b_psram)
      //    memcpy_dma (Buffer, ring_buf + (idxOUT * BUF_BYTE_CHUNK_SIZE),
      //                BUF_BYTE_CHUNK_SIZE);
      //  else
      //    memcpy (Buffer, ring_buf + (idxOUT * BUF_BYTE_CHUNK_SIZE),
      //            BUF_BYTE_CHUNK_SIZE);
      bb_get = ring_buf + (idxOUT * BUF_BYTE_CHUNK_SIZE);
      // printf (" PopRingBuf address= %d, idxOUT = %d, idxUSE=%d
      // \n\n",(int)bb_get,idxOUT,idxUSE);
      idxOUT++;
      idxOUT &= BUF_MASK;
      idxUSE--;
    }
  taskEXIT_CRITICAL ();
  return bb_get;
}

void
ResetRingBuf (void)
{
  taskENTER_CRITICAL ();
  //Сбросить данные буфера (т. е. удалить их оттуда) тоже очень просто - для
  //этого в idxOUT записывают значение idxIN: idxOUT = idxIN;
  idxIN = 0;
  idxOUT = 0;
  idxUSE = 0;
  printf (" ResetRingBuf address= %d, size %d \n\n", (unsigned int)ring_buf,
          BUF_SIZE * BUF_BYTE_CHUNK_SIZE);
  taskEXIT_CRITICAL ();
  // printf (" ResetRingBuf \n\n");
}

//сколько данных в буфере.
// static u8
// idxDiff ()
//{
//  return (idxIN - idxOUT) & BUF_MASK;
//
//    if (idxIN >= idxOUT)
//        return (idxIN - idxOUT);
//    else
//        return ((BUF_SIZE - idxOUT) + idxIN);
//
//}

int
RingBufSpacesAvailable (void)
{
  int i_size = 0;
  // printf (" RingBufSpacesAvailable idxIN = %d idxOUT = %d = %d\n\n",idxIN ,
  // idxOUT, ((idxIN - idxOUT) & BUF_MASK));
  //  return BUF_SIZE - ((idxIN - idxOUT) & BUF_MASK) - 1;
  // if((BUF_SIZE - idxUSE - 4)<=0) return 0;
  taskENTER_CRITICAL ();
  i_size = (BUF_SIZE - idxUSE); // * BUF_BYTE_CHUNK_SIZE;
  taskEXIT_CRITICAL ();
  return i_size;
}
