


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


#include "UTFT.h"

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

extern uint8_t SmallFont[]; // подключаем маленький шрифт
extern uint8_t BigFont[];   // подключаем большой шрифт
extern uint8_t
    SevenSegNumFont[]; // подключаем шрифт имитирующий семисегментный индикатор
extern uint8_t SmallSymbolFont[];


#include "wm_gpio_afsel.h"
#include "ff.h"

FRESULT
scan_files (
    char *path /* Start node to be scanned (***also used as work area***) */
)
{ /* http://elm-chan.org/fsw/ff/doc/readdir.html */
  FRESULT res;
  DIR dir;
  UINT i;
  static FILINFO fno;

  res = f_opendir (&dir, path); /* Open the directory */
  if (res == FR_OK)
    {
      for (;;)
        {
          res = f_readdir (&dir, &fno); /* Read a directory item */
          if (res != FR_OK || fno.fname[0] == 0)
            break; /* Break on error or end of dir */
          if (fno.fattrib & AM_DIR)
            { /* It is a directory */
              i = strlen (path);
              sprintf (&path[i], "/%s", fno.fname);
              res = scan_files (path); /* Enter the directory */
              if (res != FR_OK)
                break;
              path[i] = 0;
            }
          else
            { /* It is a file. */
              printf ("%s/%s\n", path, fno.fname);
              char FileName[256];
              if (strlen (path) != 0)
                sprintf (FileName, "0:%s/%s", path, fno.fname);
              else
                sprintf (FileName, "0:%s", fno.fname);
              if (strstr (FileName, "240x240") != NULL)
               {
               UTFT_loadBitmap (
                      0, 0, 240, 240,
                      FileName); // выводим на дисплей картинку
                                 // начиная с координаты 0,0 размером
                                 // 172,320 из файла 0-x.raw
               UTFT_setFont (SmallFont);
               UTFT_setColor2 (VGA_FUCHSIA);
               UTFT_print (fno.fname, CENTER, 200, 0);
               tls_os_time_delay (HZ * 3);
               }
            }
        }
      f_closedir (&dir);
    }

  return res;
}


void
user_app1_task (void *sdata)
{
  printf ("user_app1_task start TFT01_3SP 240x240 SPI st7789\n");

  // подключаем библиотеку UTFT

  // TFT01_3SP - для ST7789
  UTFT_UTFT (TFT01_3SP
 , (u8)NO_GPIO_PIN  //WM_IO_PB_17  //RS  SDA
 , (u8)NO_GPIO_PIN  //WM_IO_PB_15  //WR  SCL
 , (u8)NO_GPIO_PIN  //WM_IO_PB_22  //CS  CS
 , (u8)WM_IO_PB_21  //RST reset RES
 , (u8)WM_IO_PB_23  //SER => DC !
 , 20000000
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



  //UTFT_UTFT(byte model, byte RS, byte WR, byte CS, byte RST, byte SER, u32 spi_freq);
  //                               byte RS,         byte WR,         byte CS,
  //                               byte RST, byte SER, u32 spi_freq

  FATFS fs;
  FRESULT res_sd;
  char buff[256]; // буффер для названия директории при сканировании файловой
                  // системы
  wm_sdio_host_config (0);


  //
  UTFT_InitLCD (LANDSCAPE); // инициируем дисплей
  //UTFT_InitLCD (PORTRAIT);
  // UTFT_clrScr();                            // стираем всю информацию с
  // дисплея
  //

  while (1)
    { //
      UTFT_clrScr (); // стираем всю информацию с дисплея
      tls_os_time_delay (HZ); // 

      UTFT_setColor2 (VGA_WHITE); // 
      for(int i=1;i<60;i++){
      UTFT_drawRect (1, 1, i*4, i*4); 
      }
 
      tls_os_time_delay (HZ*3); // 
  
      UTFT_fillScr2 (VGA_BLACK);

      UTFT_setColor2 (VGA_GREEN); // Устанавливаем зелёный цвет
      UTFT_drawRect (5, 5, 160-5,
                     80-5); // Рисуем прямоугольник (с противоположными углами)
      tls_os_time_delay (HZ); // 

      UTFT_setColor2 (VGA_RED); // Устанавливаем красный цвет
      UTFT_drawLine (
          1, 1, 150,
          70); // Рисуем линию 
      tls_os_time_delay (HZ); // 

      UTFT_setColor2 (VGA_BLUE); // Устанавливаем синий цвет
      UTFT_drawRoundRect (
          10, 10, 200,
          230); // Рисуем прямоугольник со скруглёнными углами (с
                // противоположными углами в координатах 10x110 - 170x210)
      tls_os_time_delay (HZ*3); // 
                //
      UTFT_setColor2 (VGA_LIME); // Устанавливаем лаймовый цвет
      UTFT_fillRect (
           10     //по горизонтали?
         , 10    // по вертикали?
         , 230 //длинна?
         , 230  //высота?
        ); // Рисуем закрашенный прямоугольник (с противоположными углами
                // в координатах 10x220 - 170x310)
      tls_os_time_delay (HZ*3); // 
                //
      UTFT_setColor2 (VGA_PURPLE); // Устанавливаем фиолетовый цвет
      UTFT_drawCircle (
          120, 120,
          100); // Рисуем окружность (с центром в точке x y  и радиусом r)

      UTFT_fillCircle (120, 120, 50); // Рисуем закрашенную окружность (с
                                      // центром в точке x y и радиусом r)

      tls_os_time_delay (HZ * 3);

      UTFT_fillScr2 (VGA_RED);
      tls_os_time_delay (HZ); // заливаем
      //   дисплей красным,     ждём 1 секунду

      UTFT_fillScr2 (VGA_GREEN);
      tls_os_time_delay (HZ); // заливаем
      //   дисплей зелёным,     ждём 1 секунду

      UTFT_fillScr2 (VGA_BLUE);
      tls_os_time_delay (HZ); // заливаем дисплей синим,       ждём 1   секунду

      UTFT_fillScr2 (VGA_SILVER);
      tls_os_time_delay (HZ); //   заливаем дисплей серебристым, ждём 1 секунду

      UTFT_fillScr2 (VGA_MAROON);
      tls_os_time_delay (HZ); // заливаем  дисплей бордовым,    ждём 1 секунду

      UTFT_fillScr2 (VGA_NAVY);
      tls_os_time_delay (HZ); // заливаем дисплей тем. синим,  ждём 1  секунду

      UTFT_clrScr (); // стираем всю информацию с дисплея
      UTFT_setFont (BigFont); // устанавливаем большой шрифт
      UTFT_setColor2 (VGA_BLUE); // устанавливаем синий цвет текста
      UTFT_print ("BigFont", CENTER, 40,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 100 точек)
      tls_os_time_delay (HZ); // заливаем дисплей тем. синим,  ждём 1  секунду
      UTFT_clrScr (); // стираем всю информацию с дисплея
      UTFT_setColor2 (VGA_RED); // устанавливаем 
      UTFT_setBackColor2 (VGA_TRANSPARENT);
      UTFT_print ("12:35", CENTER, 40,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 115 точек)
      tls_os_time_delay (HZ * 3);
      //

      UTFT_clrScr (); // стираем всю информацию с дисплея
      UTFT_setFont (SmallFont); // устанавливаем большой шрифт
      UTFT_print ("SmallFontTest", CENTER, 10,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 100 точек)
      UTFT_print ("12345678", CENTER, 50,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 115 точек)
      tls_os_time_delay (HZ * 3);
      //
      UTFT_setFont (SevenSegNumFont); // устанавливаем шрифт имитирующий
                                      // семисегментный индикатор
      UTFT_clrScr (); // стираем всю информацию с дисплея
      UTFT_setColor2 (VGA_FUCHSIA); // устанавливаем пурпурный цвет текста
      UTFT_print ("12345", CENTER, 10,
                  0); // выводим текст на дисплей (выравнивание по ширине -
                      // центр дисплея, координата по высоте 150 точек)
      tls_os_time_delay (HZ * 3);


      UTFT_setFont (SmallFont); // устанавливаем большой шрифт
      UTFT_print ("SmallFont", CENTER, 80,
                  0); // выводим текст на дисплей (выравнивание по ширине -

      UTFT_setFont (SmallSymbolFont); // устанавливаем шрифт имитирующий
      UTFT_print ("\x20\x21\x22\x23\x24\x25", CENTER, 100,
                  0); // выводим текст на дисплей (выравнивание по ширине -
      tls_os_time_delay (HZ * 3);

      u32 current_tick = tls_os_get_time();
      u32 count=0;
      while( (tls_os_get_time() - current_tick) <= (HZ*10) )
       {
       UTFT_fillScr2 (count);
       count++;
       }
      UTFT_clrScr ();
      UTFT_setFont (BigFont);
      UTFT_setColor2 (VGA_FUCHSIA); // устанавливаем пурпурный цвет текста
      char mesg[50];
      UTFT_print ("run 10 sec", CENTER, 20, 0);       
      sprintf (mesg, "=%d FPS=%d", count, count/10);
      UTFT_print (mesg, CENTER, 50, 0);       
      tls_os_time_delay (HZ * 5);




      // mount SD card
      res_sd = f_mount (&fs, "0:", 1);
      //***********************formatting test****************************
      if (res_sd == FR_NO_FILESYSTEM)
        {
          printf ("FR_NO_FILESYSTEM:Failed to mount file system! Probably "
                  "because the file "
                  "initialization failed! error code:%d\r\n",
                  res_sd);
        }
      else if (res_sd != FR_OK)
        {
          printf ("Failed to mount file system! Probably because the file "
                  "initialization failed! error code:%d\r\n",
                  res_sd);
        }
      else
        {
          printf ("The file system is successfully mounted, and the read and "
                  "write test can be performed!\r\n");
          memset (buff, 0, sizeof (buff));
          strcpy (buff, "/");
          res_sd = scan_files (buff);
        }
      if (res_sd == FR_OK)
        {
          memset (buff, 0, sizeof (buff));
          strcpy (buff, "/");
          res_sd = scan_files (buff);
        }
      // unmount file system
      f_mount (NULL, "0:", 1);
      tls_os_time_delay (HZ * 1);



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
