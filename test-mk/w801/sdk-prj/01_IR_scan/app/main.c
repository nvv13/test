/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date : 2022-06-05
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "wm_type_def.h"
//#include "wm_uart.h"
#include "wm_gpio.h"
//#include "wm_hostspi.h"
//#include "wm_socket.h"
//#include "wm_sockets.h"
//#include "wm_wifi.h"
//#include "wm_hspi.h"
//#include "wm_pwm.h"
//#include "wm_params.h"
#include "wm_osal.h"
//#include "wm_netif.h"
//#include "wm_efuse.h"
//#include "wm_mem.h"
#include "wm_cpu.h"
#include "wm_regs.h"
#include "wm_rtc.h"
#include "wm_timer.h"
#include "wm_watchdog.h"
//#include "csi_core.h"

#include "HD44780LCD.h"
#include "n_delay.h"

#ifndef UINT32_MAX
#define UINT32_MAX (4294967295U)
#endif

#define TIC_PAUSE 100000 // us

static volatile u32 u32_us_tic = 0;
static volatile int i_tic_pause = 0;

static volatile u32 u32_start = 0;
#define RAWBUF_LEN 256
static volatile int i_rawbuf[RAWBUF_LEN]; // raw data
static volatile u16 u16_raw_buf_pos = 0;

#define ST_IDLE 0
#define ST_START 1
#define ST_LOAD1 2
#define ST_LOAD2 3
#define ST_LOAD21 4
#define ST_LOAD3 5
static volatile u8 u8_status = ST_IDLE;

static volatile u32 u32_scan_code = 0;
static volatile u8 u8_scan_pos = 0;
static volatile u16 u16_total_count=0;

static void
tic_timer_irq (u8 *arg) //
{
  u32_us_tic++;
  if (i_tic_pause >= 0)
    {
      if (i_tic_pause++ > TIC_PAUSE)
        {
          i_tic_pause = -1;
          if (u16_raw_buf_pos > 0)
            {
              // printf ("i_rawbuf[%d]: ", u16_raw_buf_pos);
              for (int i = 0; i < u16_raw_buf_pos; i++)
                {
                  // printf (",%d", i_rawbuf[i]);
                  if (u8_status == ST_LOAD21)
                    u8_status = ST_LOAD2;

                  if (u8_status == ST_LOAD3)
                    {
                      if (i_rawbuf[i] > -650 && i_rawbuf[i] < -100)
                        {
                          u8_status = ST_LOAD21;
                          if (i_rawbuf[i] > -300)
                            u8_scan_pos++; // printf ("0"); // bit=0
                          else
                            {
                              u32_scan_code
                                  = u32_scan_code
                                    | 1 << u8_scan_pos; // printf ("1"); //
                                                        // bit=1
                              u8_scan_pos++;
                            }
                        }
                      else
                        u8_status = ST_IDLE;
                    }

                  if (u8_status == ST_LOAD2)
                    {
                      if (i_rawbuf[i] > 150 && i_rawbuf[i] < 250)
                        u8_status = ST_LOAD3;
                      else
                        u8_status = ST_IDLE;
                    }

                  if (u8_status == ST_LOAD1 && i_rawbuf[i] > -2000
                      && i_rawbuf[i] < -1000)
                    u8_status = ST_LOAD2;

                  if (u8_status == ST_START && i_rawbuf[i] > 2500
                      && i_rawbuf[i] < 3500)
                    u8_status = ST_LOAD1;

                  if (u8_status == ST_IDLE && i_rawbuf[i] < -10000)
                    {
                      u8_status = ST_START;
                      printf ("0x%08x\n", u32_scan_code);
                      HD44780LCD_LCDGOTO (LCDLineNumberOne, 0);
                      HD44780LCD_printf ("0x%08x  %d", u32_scan_code, ++u16_total_count);
                      u32_scan_code = 0;
                      u8_scan_pos = 0;
                    }
                }
            }
          u16_raw_buf_pos = 0;
        }
    }
}

#define PA1_ISR_IO_TSOP48 WM_IO_PA_01
static void
isr_callback_tsop48 (void *context)
{
  u16 ret = tls_get_gpio_irq_status (PA1_ISR_IO_TSOP48);
  if (ret)
    {
      i_tic_pause = 0;
      int i_cnt_tic = 0;
      if (u32_us_tic < u32_start)
        i_cnt_tic = UINT32_MAX - u32_us_tic + u32_start;
      else
        i_cnt_tic = u32_us_tic - u32_start;

      tls_clr_gpio_irq_status (PA1_ISR_IO_TSOP48);

      if (tls_gpio_read (PA1_ISR_IO_TSOP48))
        i_rawbuf[u16_raw_buf_pos] = i_cnt_tic;
      else
        i_rawbuf[u16_raw_buf_pos] = i_cnt_tic * -1;
      if (u16_raw_buf_pos++ > RAWBUF_LEN)
        u16_raw_buf_pos = 0;
      u32_start = u32_us_tic;
    }
}

static volatile int i_tsop = 0;

const u8 u8_row = 2;
const u8 u8_col = 16;

void
UserMain (void)
{
  printf ("user task LCD test u8_row=%d, u8_col=%d\n", u8_row, u8_col);

  // tls_sys_clk_set (CPU_CLK_240M);

  HD44780LCD_HD44780LCD (u8_row, u8_col); // instantiate an object

  HD44780LCD_LCDInit (LCDCursorTypeOn);
  HD44780LCD_LCDClearScreen ();
  HD44780LCD_LCDBackLightSet (true);

  u8 timer_id;
  struct tls_timer_cfg timer_cfg;

  // timer_cfg.unit = TLS_TIMER_UNIT_MS;
  timer_cfg.unit = TLS_TIMER_UNIT_US; //
  timer_cfg.timeout = 1;              // 1 us
  timer_cfg.is_repeat = 1;
  timer_cfg.callback = (tls_timer_irq_callback)tic_timer_irq;
  timer_cfg.arg = NULL;
  timer_id = tls_timer_create (&timer_cfg);
  tls_timer_start (timer_id);
  printf ("timer start\n");

  //
  tls_gpio_cfg (PA1_ISR_IO_TSOP48, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
  tls_gpio_isr_register (PA1_ISR_IO_TSOP48, isr_callback_tsop48, NULL);
  tls_gpio_irq_enable (
      PA1_ISR_IO_TSOP48,
      WM_GPIO_IRQ_TRIG_DOUBLE_EDGE); /**< both rising edge and falling edge
                                        arise the interrupt */
  printf ("\nPA1_ISR_IO_TSOP48 gpio %d DOUBLE_EDGE isr\n", PA1_ISR_IO_TSOP48);

  for (;;)
    {
    }
}
