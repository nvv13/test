/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: pin PB1, freq = 5,714309 MHz CPU_CLK_240M
 *
 * Date : 2022-05-20
 *****************************************************************************/

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

//**********************************************************************************************************************************//

static volatile u8 u8_fill = 0;
static volatile u8 u8_check = 0;
static volatile int i_err_cnt = 0;
static volatile int i_total_read64 = 0;

static u8 buf_fill[BUF_BYTE_CHUNK_SIZE];

void
fill_buf (void)
{
  int i_rand = rand () % RingBufChunkTotal ();
  // printf (" +%d", i_rand);
  while (1)
    {
      // u32 nSize = BUF_BYTE_CHUNK_SIZE;
      size_t freeSize = RingBufChunkAvailable ();
      //printf ("free=%d i_rand=%d", freeSize, i_rand);
      if (freeSize == 0 || i_rand-- < 0)
        {
          // printf ("\r\n");
          break;
        }
      u8_fill++;
      //u8 buf_fill[BUF_BYTE_CHUNK_SIZE];
      for (int iPos = 0; iPos < BUF_BYTE_CHUNK_SIZE; iPos++)
        {
          *(buf_fill + iPos) = u8_fill;
        }
      //printf ("adr=%d ", (unsigned long)buf_fill);
      PushRingBufChunk (buf_fill);
      //printf ("+");
    }
}

static u8 buffer[BUF_BYTE_CHUNK_SIZE];

void
print_buf (void)
{
  int i_rand = rand () % RingBufChunkTotal ();
  // printf (" -%d", i_rand);
  int i_cnt = 0;
  while (1)
    {
      if (i_rand-- < 0)
        break;
      //u8 buffer[BUF_BYTE_CHUNK_SIZE];
      u8 u8_len = PopRingBufChunk (buffer);
      if (u8_len != 0)
        {
          u8_check++;
          i_cnt++;
          i_total_read64++;
          for (int iPos = 0; iPos < BUF_BYTE_CHUNK_SIZE; iPos++)
            {
              u8 u8_out = *(buffer + iPos);
              // printf ("%0.2x", u8_out);
              if (u8_check == u8_out)
                ; // printf ("%x", u8_out);
              else
                {
                  i_err_cnt++;
                  printf (" [%d]%x<>%x ", iPos, u8_out, u8_check);
                  // printf ("e");
                }
            }
          //printf ("0");
          // printf (" cnt_buf=%d address= %d\r\n", i_cnt, (unsigned
          // long)buffer);
        }
      else
        break;
    }
}

//**********************************************************************************************************************************//

#define VS1053_TASK_SIZE 2048
static tls_os_task_t vs1053_buf_play_task_hdl = NULL;
static OS_STK vs1053_buf_playTaskStk[VS1053_TASK_SIZE];
#define VS1053_TASK_PRIO 32

static void
vs1053_buf_play_task (void *sdata)
{
  printf ("start vs1053_buf_play_task\n");

  while (1)
    {
      // size_t item_size;
      while (HeadRingBuf () != NULL)
        {
          print_buf ();
          //tls_os_time_delay (HZ / 10);
        }
      tls_os_time_delay (HZ / 100);
    }

  // if (buffer != NULL)
  //  tls_mem_free (buffer);

  printf ("stop vs1053_buf_play_task\n");
}

void
UserMain (void)
{
  int i_read=0;

  printf ("test ring buf\r\n");

  tls_sys_clk_set (CPU_CLK_240M);

  unsigned int t=0; // used to save time relative to 1970
  struct tm *tblock;
  tblock = localtime ((const time_t *)&t); // switch to local time
  tls_set_rtc (tblock);
  struct tm tstart;
  struct tm tstop;


  CreateRingBuf ();

  srand (tls_os_get_time ()); // time(NULL));
  u8_fill = 0;
  u8_check = 0;
  i_err_cnt = 0;

  if (vs1053_buf_play_task_hdl == NULL)
    {
      //printf ("2\r\n");
      tls_os_task_create (
          &vs1053_buf_play_task_hdl, NULL, vs1053_buf_play_task, NULL,
          (void *)vs1053_buf_playTaskStk,  /* task's stack start address */
          VS1053_TASK_SIZE * sizeof (u32), /* task's stack size, unit:byte */
          VS1053_TASK_PRIO, 0);
    }
  tls_os_time_delay (HZ);
  printf ("3\r\n");
  tls_get_rtc (&tstart);

  while (1)
    {
      fill_buf ();
      //printf ("4\r\n");
      //tls_os_time_delay (HZ);

      //ждем свободное место в буфере,
      while (1)
        {
          if (RingBufChunkAvailable () > 0)
            break;
          tls_os_time_delay (HZ / 100);
        }

      if (i_err_cnt > 0)
        printf ("\r\ntest Error i_total_read64=%d\r\n",i_total_read64);
      else
        {
        if((i_total_read64-i_read)>(1024*1024))
           {
             tls_get_rtc (&tstop);
             int sec=(tstop.tm_hour*3600 + tstop.tm_min*60 + tstop.tm_sec) - (tstart.tm_hour*3600 + tstart.tm_min*60 + tstart.tm_sec);
             printf ("\r\ntest Ok,  i_total_read64=%d,  byte/sec=%d\r\n",i_total_read64, i_total_read64/sec);
             i_read=i_total_read64;
           }
        }
    }
  ResetRingBuf ();
}
