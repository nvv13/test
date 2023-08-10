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

static u8 buf_fill[BUF_BYTE_CHUNK_SIZE];

void
fill_buf (void)
{
  int i_rand = rand () % BUF_SIZE;
  // printf (" +%d", i_rand);
  while (1)
    {
      // u32 nSize = BUF_BYTE_CHUNK_SIZE;
      size_t freeSize = RingBufSpacesAvailable ();
      // printf ("free=%d", freeSize);
      if (freeSize == 0 || i_rand-- < 0)
        {
          // printf ("\r\n");
          break;
        }
      u8_fill++;
      // printf ("adr=%d ", (unsigned long)buf_fill);
      for (int iPos = 0; iPos < BUF_BYTE_CHUNK_SIZE; iPos++)
        {
          *(buf_fill + iPos) = u8_fill;
        }
      PushRingBuf (buf_fill);
      printf ("+");
    }
}

static u8 buffer[BUF_BYTE_CHUNK_SIZE];

void
print_buf (void)
{
  int i_rand = rand () % BUF_SIZE;
  // printf (" -%d", i_rand);
  int i_cnt = 0;
  while (1)
    {
      if (i_rand-- < 0)
        break;
      u8 u8_len = PopRingBuf (buffer);
      if (u8_len != 0)
        {
          u8_check++;
          i_cnt++;
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
          printf ("0");
          // printf (" cnt_buf=%d address= %d\r\n", i_cnt, (unsigned
          // long)buffer);
        }
      else
        break;
    }
}

//**********************************************************************************************************************************//

#define VS1053_TASK_SIZE 1024
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
          // tls_os_time_delay (HZ / 2);
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
  printf ("test ring buf\r\n");

  tls_sys_clk_set (CPU_CLK_240M);

  CreateRingBuf ();

  srand (tls_os_get_time ()); // time(NULL));
  u8_fill = 0;
  u8_check = 0;
  i_err_cnt = 0;

  if (vs1053_buf_play_task_hdl == NULL)
    {
      tls_os_task_create (
          &vs1053_buf_play_task_hdl, NULL, vs1053_buf_play_task, NULL,
          (void *)vs1053_buf_playTaskStk,  /* task's stack start address */
          VS1053_TASK_SIZE * sizeof (u32), /* task's stack size, unit:byte */
          VS1053_TASK_PRIO, 0);
    }

  while (1)
    {
      fill_buf ();
      // tls_os_time_delay (HZ);

      //ждем свободное место в буфере,
      while (1)
        {
          if (RingBufSpacesAvailable () > 0)
            break;
          tls_os_time_delay (HZ / 100);
        }

      if (i_err_cnt > 0)
        printf ("\r\n\r\ntest Error\r\n");
      else
        printf ("\r\n\r\ntest Ok\r\n");
    }
  ResetRingBuf ();
}
