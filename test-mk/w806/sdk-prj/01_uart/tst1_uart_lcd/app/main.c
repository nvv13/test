/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date :
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"

#include "wm_cpu.h"

#include "wm_adc.h"
#include "wm_gpio_afsel.h"

#include "wm_gpio.h"
#include "wm_watchdog.h"
//#include "wm_pwm.h"
//#include "wm_params.h"
#include "wm_osal.h"
#include "wm_timer.h"
//#include "wm_efuse.h"
//#include "wm_mem.h"
//#include "wm_regs.h"
#include "wm_rtc.h"
#include "wm_uart.h"

#include "mod1/u8g2.h"
#include "mod1/u8x8_riotos.h"

// extern void dumpBuffer (char *name, char *buffer, int len);

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

extern uint8_t SmallFont[]; // подключаем маленький шрифт
extern uint8_t BigFont[];   // подключаем большой шрифт
extern uint8_t
    SevenSegNumFont[]; // подключаем шрифт имитирующий семисегментный индикатор
extern uint8_t SmallSymbolFont[];
extern uint8_t Dingbats1_XL[];

#define CONSOLE_BUF_SIZE 512
u8 rx_buf[CONSOLE_BUF_SIZE + 1];
volatile int rx_data_len;
volatile u32 rptr;

s16
proc_console_rx (u16 len, void *user_data)
{
  rx_data_len += len;
  return 0;
}

void
user_app1_task (void *sdata)
{

  tls_watchdog_init (30 * 1000 * 1000); // u32 usec microseconds, около 30 сек

  // подключаем библиотеку

  uint32_t line_screen = 0;
  u8g2_t u8g2;

  /* initialize to SPI */
  puts ("Initializing to SPI.");

  u8x8_riotos_t user_data = {

    .pin_cs = WM_IO_PB_22,    // GPIO_PIN(PORT_A, 4),
    .pin_dc = WM_IO_PB_23,    // GPIO_UNDEF,
    .pin_reset = WM_IO_PB_21, // GPIO_PIN(PORT_A, 0),

    .spi_cs = WM_IO_PB_14, /* */
    .spi_ck = WM_IO_PB_15, /* */
    .spi_di = WM_IO_PB_16, /* */
    .spi_do = WM_IO_PB_17, /* */
  };

  u8g2_SetUserPtr (&u8g2, &user_data);

  u8g2_Setup_ssd1322_nhd_256x64_1 (&u8g2, U8G2_R0, u8x8_byte_hw_spi_riotos,
                                   u8x8_gpio_and_delay_riotos);

  u8g2_SetUserPtr (&u8g2, &user_data);

  /* initialize the display */
  puts ("Initializing display.");

  u8g2_InitDisplay (&u8g2);
  u8g2_SetPowerSave (&u8g2, 0);

  /* start drawing in a loop */
  puts ("Drawing on screen.");

  memset (rx_buf, 0, CONSOLE_BUF_SIZE + 1);
  rx_data_len = 0;
  rptr = 0;

  tls_uart_set_baud_rate (TLS_UART_0, 115200);
  tls_uart_rx_callback_register (TLS_UART_0, proc_console_rx, NULL);

  wm_adc_config (0);
  wm_adc_config (1);
  int voltage0 = 0;
  int voltage1 = 0;

  while (1)
    {
      tls_os_time_delay (HZ / 4);
      tls_watchdog_clr (); // пока есть управление, не перезапускаем!

      if (rx_data_len > 0)
        {
          if (rptr + rx_data_len > CONSOLE_BUF_SIZE)
            {
              printf ("rptr + rx_data_len > CONSOLE_BUF_SIZE\r\n");
              memset (rx_buf, 0, CONSOLE_BUF_SIZE + 1);
              rx_data_len = 0;
              rptr = 0;
            }
          else
            {
              int ret = tls_uart_read (TLS_UART_0, rx_buf + rptr, rx_data_len);
              if (ret > 0)
                {
                  // printf ("%s", (char *)(rx_buf + rptr));
                  // dumpBuffer ("\ruart0", (char *)rx_buf, 64);
                  char *pHeaderEnd = strstr ((char *)(rx_buf + rptr), "\r");
                  rx_data_len -= ret;
                  rptr += ret;
                  if (pHeaderEnd)
                    {
                      (*pHeaderEnd) = 0;
                      pHeaderEnd = (char *)rx_buf;
                      printf ("buf = \"%s\"\r\n", pHeaderEnd);

                      u8g2_FirstPage (&u8g2);
                      do
                        {
                          u8g2_SetDrawColor (&u8g2, 1);
                          u8g2_SetFont (&u8g2, u8g2_font_helvB12_tf);
                          u8g2_DrawStr (&u8g2, 4, 2 + 20 * line_screen,
                                        pHeaderEnd);
                        }
                      while (u8g2_NextPage (&u8g2));

                      memset (rx_buf, 0, CONSOLE_BUF_SIZE + 1);
                      rx_data_len = 0;
                      rptr = 0;
                      if (line_screen++ > 3)
                        line_screen = 0;
                    }
                }
            }
        }

      voltage0 = adc_get_inputVolt (0);
      voltage1 = adc_get_inputVolt (1);

      u8g2_FirstPage (&u8g2);
      do
        {
          // void u8g2_DrawBox(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y,
          // u8g2_uint_t w, u8g2_uint_t h);
          u8g2_DrawBox (&u8g2, 7, 10, 10 + voltage0, 5);
          u8g2_DrawBox (&u8g2, 12, 10, 10 + voltage1, 5);
        }
      while (u8g2_NextPage (&u8g2));
    }
}

void
UserMain (void)
{
  printf ("UserMain start");
  tls_sys_clk_set (CPU_CLK_240M);
  // tls_sys_clk_set (CPU_CLK_2M);
  // tls_sys_clk_set (CPU_CLK_40M);
  // tls_sys_clk_set (CPU_CLK_80M);
  // tls_sys_clk_set (CPU_CLK_160M);

  tls_os_task_create (NULL, NULL, user_app1_task, NULL,
                      (void *)UserApp1TaskStk, /* task's stack start address */
                      USER_APP1_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP1_TASK_PRIO, 0);
}
