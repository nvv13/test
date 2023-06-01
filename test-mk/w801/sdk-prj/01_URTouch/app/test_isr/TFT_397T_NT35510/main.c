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

#include "URTouch.h"
#include "mod1/UTFT.h"

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

extern uint8_t SmallFont[]; // подключаем маленький шрифт
extern uint8_t BigFont[];   // подключаем большой шрифт
extern uint8_t
    SevenSegNumFont[]; // подключаем шрифт имитирующий семисегментный индикатор
extern uint8_t SmallSymbolFont[];


#define TOUCH_IRQ WM_IO_PA_09
static volatile u8 flag_touch_isr=false;
static void isr_callback (void *context)
{
  u16 ret = tls_get_gpio_irq_status (TOUCH_IRQ);
  if (ret)
    {
      tls_clr_gpio_irq_status (TOUCH_IRQ);
      flag_touch_isr=true;
      //if (i_dreb_SW == 0) // защита от ддребезга контактов для кнопки
      //  {
      //    i_dreb_SW = 1;
      //  }
    }
}


void
user_app1_task (void *sdata)
{
  printf ("user_app1_task start 3.97 TFT_397T_NT35510 800x480 16bit bus\n");

  // Цветной графический дисплей 3.97 TFT_397T_NT35510 с тачскрином
  // подключаем библиотеку UTFT
  UTFT_UTFT (TFT_397T_NT35510, (u8)WM_IO_PA_01, (u8)WM_IO_PA_02,
             (u8)WM_IO_PA_03, (u8)WM_IO_PA_04, 0, 0);
  //                               byte RS,         byte WR,         byte CS,
  //                               byte RST, byte SER, u32 spi_freq
  // и номера выводов W801 к которым подключён дисплей: RS, WR,
  // CS, RST. Выводы параллельной шины данных не указываются
  // в данном случае, параллельная 16 бит шина = PB0 ... PB15
  UTFT_InitLCD (LANDSCAPE); // инициируем дисплей

  UTFT_clrScr (); // стираем всю информацию с дисплея

  UTFT_setColor2 (VGA_WHITE); // 800x480
  for (int i = 2; i < 160; i++)
    {
      UTFT_drawRect (2, 2, i * 5, i * 3);
    }
  tls_os_time_delay (HZ*3); //

  // URTouch_URTouch(byte tclk, byte tcs, byte tdin, byte dout, byte irq);
  URTouch_URTouch ((u8)WM_IO_PA_05 // byte tclk
                   ,
                   (u8)WM_IO_PA_06 // byte tcs
                   ,
                   (u8)WM_IO_PA_07 // byte tdin
                   ,
                   (u8)WM_IO_PA_08 // byte dout
                   ,
                   (u8)TOUCH_IRQ // byte irq
  );

  URTouch_InitTouch (LANDSCAPE);
  URTouch_setPrecision (PREC_MEDIUM);

  URTouch_register_irq_callback_func(isr_callback);
  //
  int x = 0, y = 0;

  UTFT_clrScr ();
  UTFT_setFont (BigFont);
  UTFT_setColor2 (VGA_FUCHSIA); // устанавливаем пурпурный цвет текста

  while (1)
    { //

      if (flag_touch_isr)//;URTouch_dataAvailable ())
        {
          flag_touch_isr=false;
          URTouch_read ();
          x = URTouch_getX ();
          y = URTouch_getY ();
          if (x >= 0 && y >= 0)
            {
              char mesg[50];
              sprintf (mesg, "X=%.3d Y=%.3d", x, y);
              UTFT_print (mesg, CENTER, 10, 0);
              UTFT_fillCircle (x, y, 2); // Рисуем закрашенную окружность
              printf ("touch X=%.3d Y=%.3d\n",x, y);
            }
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
