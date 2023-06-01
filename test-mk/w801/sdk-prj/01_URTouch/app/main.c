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

#define TOUCH_ORIENTATION PORTRAIT
//#define TOUCH_ORIENTATION  LANDSCAPE

// Declare which fonts we will be using
extern uint8_t SmallFont[];

uint32_t cx, cy;
uint32_t rx[8], ry[8];
uint32_t clx, crx, cty, cby;
float px, py;
int dispx, dispy, text_y_center;
uint32_t calx, caly, cals;
char buf[13];

void
drawCrossHair (int x, int y)
{
  UTFT_drawRect (x - 10, y - 10, x + 10, y + 10);
  UTFT_drawLine (x - 5, y, x + 5, y);
  UTFT_drawLine (x, y - 5, x, y + 5);
}

void
fail ()
{
  UTFT_clrScr ();
  UTFT_setColor (255, 0, 0);
  UTFT_fillRect (0, 0, dispx - 1, 13);
  UTFT_setColor2 (VGA_LIME);//UTFT_setColor (255, 255, 255);
  UTFT_setBackColor (255, 0, 0);
  UTFT_drawLine (0, 14, dispx - 1, 14);
  UTFT_print ("URTouch Calibration FAILED", CENTER, 1, 0);
  UTFT_setBackColor (0, 0, 0);

  UTFT_print ("Unable to read the position", LEFT, 30, 0);
  UTFT_print ("of the press. This is a", LEFT, 42, 0);
  UTFT_print ("hardware issue and can", 88, 54, 0);
  UTFT_print ("not be corrected in", LEFT, 66, 0);
  UTFT_print ("software.", LEFT, 78, 0);

  while (true)
    {
    };
}

void
readCoordinates ()
{
  int iter = 5000;
  int failcount = 0;
  int cnt = 0;
  uint32_t tx = 0;
  uint32_t ty = 0;
  boolean OK = false;

  while (OK == false)
    {
      UTFT_setColor2 (VGA_LIME);//UTFT_setColor (255, 255, 255);
      UTFT_print ("*  PRESS  *", CENTER, text_y_center, 0);
      while (URTouch_dataAvailable () == false)
        {
        }
      UTFT_print ("*  HOLD!  *", CENTER, text_y_center, 0);
      while ((URTouch_dataAvailable () == true) && (cnt < iter)
             && (failcount < 10000))
        {
          URTouch_calibrateRead ();
          if (!((URTouch_TP_X == 65535) || (URTouch_TP_Y == 65535)))
            {
              tx += URTouch_TP_X;
              ty += URTouch_TP_Y;
              cnt++;
            }
          else
            failcount++;
        }
      if (cnt >= iter)
        {
          OK = true;
        }
      else
        {
          tx = 0;
          ty = 0;
          cnt = 0;
        }
      if (failcount >= 10000)
        fail ();
    }

  cx = tx / iter;
  cy = ty / iter;
}

void
calibrate (int x, int y, int i)
{
  UTFT_setColor2 (VGA_LIME);//UTFT_setColor (255, 255, 255);
  drawCrossHair (x, y);
  UTFT_setBackColor (255, 0, 0);
  readCoordinates ();
  UTFT_setColor2 (VGA_LIME);//UTFT_setColor (255, 255, 255);
  UTFT_print ("* RELEASE *", CENTER, text_y_center, 0);
  UTFT_setColor (80, 80, 80);
  drawCrossHair (x, y);
  rx[i] = cx;
  ry[i] = cy;
  while (URTouch_dataAvailable () == true)
    {
    }
}

void
waitForTouch ()
{
  while (URTouch_dataAvailable () == true)
    {
    }
  while (URTouch_dataAvailable () == false)
    {
    }
  while (URTouch_dataAvailable () == true)
    {
    }
}

void
toHex (uint32_t num)
{
  buf[0] = '0';
  buf[1] = 'x';
  buf[10] = 'U';
  buf[11] = 'L';
  buf[12] = 0;
  for (int zz = 9; zz > 1; zz--)
    {
      if ((num & 0xF) > 9)
        buf[zz] = (num & 0xF) + 55;
      else
        buf[zz] = (num & 0xF) + 48;
      num = num >> 4;
    }
}

void
startup ()
{
  UTFT_setColor (255, 0, 0);
  UTFT_fillRect (0, 0, dispx - 1, 13);
  UTFT_setColor2 (VGA_LIME); // UTFT_setColor(255, 255, 255);
  UTFT_setBackColor (255, 0, 0);
  UTFT_drawLine (0, 14, dispx - 1, 14);
  UTFT_print ("URTouch Calibration", CENTER, 1, 0);
  UTFT_setBackColor (0, 0, 0);

  if (dispx == 220)
    {
      UTFT_print ("Use a stylus or something", LEFT, 30, 0);
      UTFT_print ("similar to touch as close", LEFT, 42, 0);
      UTFT_print ("to the center of the", LEFT, 54, 0);
      UTFT_print ("highlighted crosshair as", LEFT, 66, 0);
      UTFT_print ("possible. Keep as still as", LEFT, 78, 0);
      UTFT_print ("possible and keep holding", LEFT, 90, 0);
      UTFT_print ("until the highlight is", LEFT, 102, 0);
      UTFT_print ("removed. Repeat for all", LEFT, 114, 0);
      UTFT_print ("crosshairs in sequence.", LEFT, 126, 0);
      UTFT_print ("Touch screen to continue", CENTER, 162, 0);
    }
  else
    {
      UTFT_print ("INSTRUCTIONS", CENTER, 30, 0);
      UTFT_print ("Use a stylus or something similar to", LEFT, 50, 0);
      UTFT_print ("touch as close to the center of the", LEFT, 62, 0);
      UTFT_print ("highlighted crosshair as possible. Keep", LEFT, 74, 0);
      UTFT_print ("as still as possible and keep holding", LEFT, 86, 0);
      UTFT_print ("until the highlight is removed. Repeat", LEFT, 98, 0);
      UTFT_print ("for all crosshairs in sequence.", LEFT, 110, 0);

      UTFT_print ("Further instructions will be displayed", LEFT, 134, 0);
      UTFT_print ("when the calibration is complete.", LEFT, 146, 0);

      UTFT_print ("Do NOT use your finger as a calibration", LEFT, 170, 0);
      UTFT_print ("stylus or the result WILL BE imprecise.", LEFT, 182, 0);

      UTFT_print ("Touch screen to continue", CENTER, 226, 0);
    }

  waitForTouch ();
  UTFT_clrScr ();
}

void
done ()
{
  UTFT_clrScr ();
  UTFT_setColor (255, 0, 0);
  UTFT_fillRect (0, 0, dispx - 1, 13);
  UTFT_setColor2 (VGA_LIME); // UTFT_setColor(255, 255, 255);
  UTFT_setBackColor (255, 0, 0);
  UTFT_drawLine (0, 14, dispx - 1, 14);
  UTFT_print ("URTouch Calibration", CENTER, 1, 0);
  UTFT_setBackColor (0, 0, 0);

  if (dispx == 220)
    {
      UTFT_print ("To use the new calibration", LEFT, 30, 0);
      UTFT_print ("settings you must edit the", LEFT, 42, 0);
      UTFT_setColor (160, 160, 255);
      UTFT_print ("URTouchCD.h", LEFT, 54, 0);
      UTFT_setColor2 (VGA_LIME); // UTFT_setColor(255, 255, 255);
      UTFT_print ("file and change", 88, 54, 0);
      UTFT_print ("the following values. The", LEFT, 66, 0);
      UTFT_print ("values are located right", LEFT, 78, 0);
      UTFT_print ("below the opening comment.", LEFT, 90, 0);
      UTFT_print ("CAL_X", LEFT, 110, 0);
      UTFT_print ("CAL_Y", LEFT, 122, 0);
      UTFT_print ("CAL_S", LEFT, 134, 0);
      toHex (calx);
      UTFT_print (buf, 75, 110, 0);
      toHex (caly);
      UTFT_print (buf, 75, 122, 0);
      toHex (cals);
      UTFT_print (buf, 75, 134, 0);

      printf ("URTouchCD.h Hex CAL_X=%X CAL_Y=%X CAL_S=%X \n", calx, caly,
              cals);
      printf ("URTouchCD.h Dec CAL_X=%d CAL_Y=%d CAL_S=%d \n", calx, caly,
              cals);
    }
  else
    {
      UTFT_print ("CALIBRATION COMPLETE", CENTER, 30, 0);
      UTFT_print ("To use the new calibration", LEFT, 50, 0);
      UTFT_print ("settings you must edit the", LEFT, 62, 0);
      UTFT_setColor (160, 160, 255);
      UTFT_print ("URTouchCD.h", LEFT, 74, 0);
      UTFT_setColor2 (VGA_LIME); // UTFT_setColor(255, 255, 255);
      UTFT_print ("file and change", 88, 74, 0);
      UTFT_print ("the following values.", LEFT, 86, 0);
      UTFT_print ("The values are located right", LEFT, 98, 0);
      UTFT_print ("below the opening comment in", LEFT, 110, 0);
      UTFT_print ("the file.", LEFT, 122, 0);
      UTFT_print ("CAL_X", LEFT, 150, 0);
      UTFT_print ("CAL_Y", LEFT, 162, 0);
      UTFT_print ("CAL_S", LEFT, 174, 0);

      toHex (calx);
      UTFT_print (buf, 75, 150, 0);
      toHex (caly);
      UTFT_print (buf, 75, 162, 0);
      toHex (cals);
      UTFT_print (buf, 75, 174, 0);

      printf ("URTouchCD.h Hex CAL_X=%X CAL_Y=%X CAL_S=%X \n", calx, caly,
              cals);
      printf ("URTouchCD.h Dec CAL_X=%d CAL_Y=%d CAL_S=%d \n", calx, caly,
              cals);
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
  UTFT_InitLCD (TOUCH_ORIENTATION); // LANDSCAPE); // инициируем дисплей

  UTFT_clrScr (); // стираем всю информацию с дисплея
  UTFT_setColor2 (VGA_LIME); // 800x480
  UTFT_setFont (SmallFont);

  // URTouch_URTouch(byte tclk, byte tcs, byte tdin, byte dout, byte irq);
  URTouch_URTouch ((u8)WM_IO_PA_05 // byte tclk
                   ,
                   (u8)WM_IO_PA_06 // byte tcs
                   ,
                   (u8)WM_IO_PA_07 // byte tdin
                   ,
                   (u8)WM_IO_PA_08 // byte dout
                   ,
                   (u8)WM_IO_PA_09 // byte irq
  );

  URTouch_InitTouch (TOUCH_ORIENTATION);
  // URTouch_setPrecision (PREC_MEDIUM);
  dispx = UTFT_getDisplayXSize ();
  dispy = UTFT_getDisplayYSize ();
  text_y_center = (dispy / 2) - 6;

  startup ();

  UTFT_setColor (80, 80, 80);
  drawCrossHair (dispx - 11, 10);
  drawCrossHair (dispx / 2, 10);
  drawCrossHair (10, 10);
  drawCrossHair (dispx - 11, dispy / 2);
  drawCrossHair (10, dispy / 2);
  drawCrossHair (dispx - 11, dispy - 11);
  drawCrossHair (dispx / 2, dispy - 11);
  drawCrossHair (10, dispy - 11);
  UTFT_setColor2 (VGA_LIME);//UTFT_setColor (255, 255, 255);
  UTFT_setBackColor (255, 0, 0);
  UTFT_print ("***********", CENTER, text_y_center - 12, 0);
  UTFT_print ("***********", CENTER, text_y_center + 12, 0);

  calibrate (10, 10, 0);
  calibrate (10, dispy / 2, 1);
  calibrate (10, dispy - 11, 2);
  calibrate (dispx / 2, 10, 3);
  calibrate (dispx / 2, dispy - 11, 4);
  calibrate (dispx - 11, 10, 5);
  calibrate (dispx - 11, dispy / 2, 6);
  calibrate (dispx - 11, dispy - 11, 7);

  if (TOUCH_ORIENTATION == LANDSCAPE)
    cals = ((long)(dispx - 1) << 12) + (dispy - 1);
  else
    cals = ((long)(dispy - 1) << 12) + (dispx - 1);

  if (TOUCH_ORIENTATION == PORTRAIT)
    px = abs ((((float)(rx[2] + rx[4] + rx[7]) / 3)
               - ((float)(rx[0] + rx[3] + rx[5]) / 3))
              / (dispy - 20)); // PORTRAIT
  else
    px = abs ((((float)(rx[5] + rx[6] + rx[7]) / 3)
               - ((float)(rx[0] + rx[1] + rx[2]) / 3))
              / (dispy - 20)); // LANDSCAPE

  if (TOUCH_ORIENTATION == PORTRAIT)
    {
      clx = (((rx[0] + rx[3] + rx[5]) / 3)); // PORTRAIT
      crx = (((rx[2] + rx[4] + rx[7]) / 3)); // PORTRAIT
    }
  else
    {
      clx = (((rx[0] + rx[1] + rx[2]) / 3)); // LANDSCAPE
      crx = (((rx[5] + rx[6] + rx[7]) / 3)); // LANDSCAPE
    }
  if (clx < crx)
    {
      clx = clx - (px * 10);
      crx = crx + (px * 10);
    }
  else
    {
      clx = clx + (px * 10);
      crx = crx - (px * 10);
    }

  if (TOUCH_ORIENTATION == PORTRAIT)
    py = abs ((((float)(ry[5] + ry[6] + ry[7]) / 3)
               - ((float)(ry[0] + ry[1] + ry[2]) / 3))
              / (dispx - 20)); // PORTRAIT
  else
    py = abs ((((float)(ry[0] + ry[3] + ry[5]) / 3)
               - ((float)(ry[2] + ry[4] + ry[7]) / 3))
              / (dispx - 20)); // LANDSCAPE

  if (TOUCH_ORIENTATION == PORTRAIT)
    {
      cty = (((ry[5] + ry[6] + ry[7]) / 3)); // PORTRAIT
      cby = (((ry[0] + ry[1] + ry[2]) / 3)); // PORTRAIT
    }
  else
    {
      cty = (((ry[0] + ry[3] + ry[5]) / 3)); // LANDSCAPE
      cby = (((ry[2] + ry[4] + ry[7]) / 3)); // LANDSCAPE
    }
  if (cty < cby)
    {
      cty = cty - (py * 10);
      cby = cby + (py * 10);
    }
  else
    {
      cty = cty + (py * 10);
      cby = cby - (py * 10);
    }

  calx = ((long)(clx) << 14) + (long)(crx);
  caly = ((long)(cty) << 14) + (long)(cby);
  if (TOUCH_ORIENTATION == LANDSCAPE)
    cals = cals + (1L << 31);

  done ();

  // URTouchCD.h Hex CAL_X=1F3C7E2 CAL_Y=1FB47EF CAL_S=8031F1DF
  // URTouchCD.h Dec CAL_X=32753634 CAL_Y=33245167 CAL_S=-2144210465

  //URTouch_set_calibrate (calx, caly, cals);

  int x = 0, y = 0;
  // UTFT_clrScr();
  while (1)
    { //

      if (URTouch_dataAvailable ())
        {
          URTouch_read ();
          x = URTouch_getX ();
          y = URTouch_getY ();
          if (x >= 0 && y >= 0)
            {
              // char mesg[50];
              // sprintf (mesg, "X=%.3d Y=%.3d", x, y);
              // UTFT_print (mesg, CENTER, 10, 0);
              UTFT_fillCircle (x, y, 2); // Рисуем закрашенную окружность
              printf ("touch X=%.3d Y=%.3d\n", x, y);
            }
        }
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
