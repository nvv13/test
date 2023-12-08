

/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date : 2023-04-11
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"

#include "wm_cpu.h"

//#include "wm_watchdog.h"
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
//#include "wm_regs.h"
#include "wm_rtc.h"

#include "UTFT.h"
#include "mod1/psram.h"
#include "u_jpeg.h"

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

extern uint8_t SmallFont[]; // подключаем маленький шрифт
extern uint8_t BigFont[];   // подключаем большой шрифт
extern uint8_t
    SevenSegNumFont[]; // подключаем шрифт имитирующий семисегментный индикатор
extern uint8_t SmallSymbolFont[];

#include "ff.h"
#include "wm_gpio_afsel.h"

void
user_app1_task (void *sdata)
{
  wm_psram_config (1);
  d_psram_init (PSRAM_SPI, 2, 2, 1, 2);
  tls_os_time_delay (HZ / 10);

  // подключаем библиотеку UTFT
  printf ("user_app1_task start TFT02_0V89 240x320 SDIO SPI st7789\n");
  UTFT_UTFT (TFT02_0V89,
             (u8)NO_GPIO_PIN // WM_IO_PB_17  // SDA
             ,
             (u8)NO_GPIO_PIN // WM_IO_PB_15  // SCL
             ,
             (u8)WM_IO_PB_14 //(u8)NO_GPIO_PIN //  //CS  CS
             ,
             (u8)WM_IO_PB_21 // RST reset RES
             ,
             (u8)WM_IO_PB_22 // SER => DC !
             ,
             // 120000000
             60000000
             /* spi_freq(Герц) для 5 контактных SPI дисплеев
                (где отдельно ножка комманда/данные)
             програмируеться HW SPI на ножки (предопред)
                 wm_spi_cs_config (WM_IO_PB_14);
                 wm_spi_ck_config (WM_IO_PB_15);
                 wm_spi_di_config (WM_IO_PB_16);
                 wm_spi_do_config (WM_IO_PB_17);
             но, можно отказаться от HW SPI в пользу Soft SPI
             установив spi_freq=0
             эмуляции SPI, это удобно для разных ножек
           */
  );

  // UTFT_InitLCD (TOUCH_ORIENTATION); // инициируем дисплей
  UTFT_InitLCD (LANDSCAPE); // инициируем дисплей
  // UTFT_InitLCD (PORTRAIT);
  bool b_Land=true;
  while (1)
    { //

      UTFT_clrScr (); // стираем всю информацию с дисплея

      UTFT_setColor2 (VGA_WHITE); // 240x280
      for (int i = 2; i < 59; i++)
        {
          UTFT_drawRect (2, 2, i * 5, i * 4);
        }

      tls_os_time_delay (HZ * 1); //

      UTFT_fillScr2 (VGA_BLACK);

      u8 *buf_prsam = UTFT_store_to_psram (true);
      int x_size = UTFT_getDisplayXSize (); // 320
      int y_size = UTFT_getDisplayYSize (); // 240

      if (1 == 0)
        {
          int xl=0;
          UTFT_InitLCD (LANDSCAPE); // инициируем дисплей
          x_size = UTFT_getDisplayXSize (); // 320
          y_size = UTFT_getDisplayYSize (); // 240
          int disp_y_size = x_size - 1;
          int disp_x_size = y_size - 1;
          int x, y, sx, sy;
          sx = 16;
          sy = 16;
          for (y = 0; y <= disp_x_size; y += sy)
            {
              xl++;
              for (x = 0; x <= disp_y_size; x += sx)
                {

                  uint16_t col = 0xffff;
                  if(xl++%2==0)col= 0x001f; // RGB565  
                  uint16_t *buf = (uint16_t *)buf_prsam;
                  int tx, ty; //, tc;

                  printf ("x=%d, y=%d, sx=%d, sy=%d, disp_x_size=%d, "
                          "disp_y_size=%d \r\n",
                          x, y, sx, sy, (int)disp_x_size, (int)disp_y_size);
                  for (ty = 0; ty < sy; ty++)
                    {
                      // UTFT_setXY (x, y + ty, x + sx - 1, y + ty);
                      for (tx = sx - 1; tx >= 0; tx--)
                        {
                          //col = 0xffff - tx; // pgm_read_word (&data[(ty * sx) +
                                        // tx]);
                          // UTFT_setPixel (col);

                          //uint8_t *s = (uint8_t *)&col;
                          //col = (((uint16_t)s[0] << 8) + (uint16_t)s[1]);

                          //*(buf + x + (y*x_size) ) = 0xffff;
                          /*
                             x= 0...304,     y= 0...224
                          disp_x_size=239, disp_y_size=319
                          */

                          //printf ("ty=%d, tx=%d, x + (sx - 1 - tx) =%d, ( (y + ty)*(disp_y_size + 1)=%d \r\n",
                          //     ty,tx,        x + (sx - 1 - tx) , ( (y + ty) * (disp_y_size + 1)));
                          buf[
                              //                                  x + tx +
                              //                                  x + tx + (sx
                              //                                  - 1 - tx)   +
                              x + (sx - 1 - tx) +
                              //                                  x  + (sx -
                              //                                  tx)   +

                                                               ( (y + ty) *
                                                               (disp_y_size +
                                                               1) )
                              //((y + (sy - ty - 1)) * (disp_y_size + 1))
                              //                               ( (y + (sy - ty
                              //                               )) *
                              //                               (disp_y_size +
                              //                               1) )

                          ] = col;
                        }
                    }
                  UTFT_psram_to_drawBitmap ();
                  //tls_os_time_delay (HZ/2); //
                }
            }
          UTFT_psram_to_drawBitmap ();
        }

      if (1 == 0)
        {
          UTFT_InitLCD (LANDSCAPE); // инициируем дисплей
          x_size = UTFT_getDisplayXSize (); // 320
          y_size = UTFT_getDisplayYSize (); // 240

          for (int y = 20; y < y_size - 80; y++)
            {
              for (int x = 40; x < x_size - 20; x++)
                {
                  *(((u16 *)buf_prsam) + x + (y * x_size)) = 0x07e0; // RGB565
                }
            }
          printf ("UTFT_psram_to_drawBitmap LANDSCAPE\r\n");
          UTFT_psram_to_drawBitmap ();
          tls_os_time_delay (HZ * 1); //
        }

      if (1 == 0)
        {
          UTFT_InitLCD (PORTRAIT);
          x_size = UTFT_getDisplayXSize (); // 240
          y_size = UTFT_getDisplayYSize (); // 320
          buf_prsam = UTFT_store_to_psram (true);

          for (int y = 20; y < y_size - 80; y++)
            {
              for (int x = 40; x < x_size - 20; x++)
                {
                  *(((u16 *)buf_prsam) + x + (y * x_size)) = 0xf800; // RGB565
                }
            }
          printf ("UTFT_psram_to_drawBitmap PORTRAIT\r\n");
          UTFT_psram_to_drawBitmap ();
          tls_os_time_delay (HZ * 1); //
          buf_prsam = UTFT_store_to_psram (true);

          for (int y = 80; y < y_size - 20; y++)
            {
              for (int x = 80; x < x_size - 80; x++)
                {
                  *(((u16 *)buf_prsam) + x + (y * x_size)) = 0x001f; // RGB565
                }
            }
          printf ("UTFT_psram_to_drawBitmap PORTRAIT 2\r\n");
          UTFT_psram_to_drawBitmap ();
          tls_os_time_delay (HZ * 1); //
        }

      if (1 == 1)
        {
          if(b_Land)
            UTFT_InitLCD (LANDSCAPE); // инициируем дисплей
          else
            UTFT_InitLCD (PORTRAIT);
          b_Land=!b_Land;
          x_size = UTFT_getDisplayXSize (); // 320
          y_size = UTFT_getDisplayYSize (); // 240
          unsigned int t = 0; // used to save time relative to 1970
          struct tm *tblock;
          tblock = localtime ((const time_t *)&t); // switch to local time
          tls_set_rtc (tblock);
          struct tm tstart;
          struct tm tstop;
          tls_get_rtc (&tstart);
          u32 current_tick = tls_os_get_time ();
          u32 count = 0;
          buf_prsam = UTFT_store_to_psram (true);
          while ((tls_os_get_time () - current_tick) <= (HZ * 10))
            {
              for (int y = count & 0x3f; y < (y_size - (count & 0x3f)); y++)
                {
                  for (int x = count & 0x3f; x < (x_size - (count & 0x3f));
                       x++)
                    {
                      *(((u16 *)buf_prsam) + x + (y * x_size))
                          = (count & 0xffff); // RGB565
                    }
                }
              UTFT_psram_to_drawBitmap ();
              count++;
            }
          tls_get_rtc (&tstop);
          int sec
              = (tstop.tm_hour * 3600 + tstop.tm_min * 60 + tstop.tm_sec)
                - (tstart.tm_hour * 3600 + tstart.tm_min * 60 + tstart.tm_sec);
          UTFT_clrScr ();
          UTFT_setFont (BigFont);
          UTFT_setColor2 (VGA_FUCHSIA); // устанавливаем пурпурный цвет текста
          char mesg[50];
          sprintf (mesg, "run %d sec", sec);
          UTFT_print (mesg, CENTER, 20, 0);
          sprintf (mesg, "=%d FPS=%d", count, count / sec);
          UTFT_print (mesg, CENTER, 50, 0);
          tls_os_time_delay (HZ * 10);
        }

    } //
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
