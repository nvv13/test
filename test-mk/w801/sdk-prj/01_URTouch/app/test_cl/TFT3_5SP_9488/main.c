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

//#define TOUCH_ORIENTATION PORTRAIT
#define TOUCH_ORIENTATION  LANDSCAPE

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

//  printf ("readCoordinates Hex cx=%X cy=%X iter=%X \n", cx, cy,  iter);
//  printf ("readCoordinates Dec cx=%d cy=%d iter=%d \n", cx, cy,  iter);
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
  printf ("calibrate Hex x=%X y=%X i=%X cx=%X cy=%X  \n" , x ,y ,  i, cx, cy);
  printf ("calibrate Dec x=%d y=%d i=%d cx=%d cy=%d  \n" , x ,y ,  i, cx, cy);
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
  printf ("user_app1_task start 3.5 TFT 320x480 HW SPI \n");

  // подключаем библиотеку UTFT
  UTFT_UTFT (TFT3_5SP_9488
             ,(u8)NO_GPIO_PIN // WM_IO_PB_17  //RS  SDA
             ,
             (u8)NO_GPIO_PIN // WM_IO_PB_15  //WR  SCL
             ,
             (u8)NO_GPIO_PIN // WM_IO_PB_14  //CS  CS
             ,
             (u8)WM_IO_PB_21 // RST reset RES
             ,
             (u8)WM_IO_PB_23 // SER => DC !
             ,
             20000000
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



  UTFT_InitLCD (TOUCH_ORIENTATION); // LANDSCAPE); // инициируем дисплей

  UTFT_clrScr (); // стираем всю информацию с дисплея
  UTFT_setColor2 (VGA_LIME); // 800x480
  UTFT_setFont (SmallFont);

  // URTouch_URTouch(byte tclk, byte tcs, byte tdin, byte dout, byte irq);
  URTouch_URTouch (TS_FT6236  // sensor_type
                   ,
                   WM_IO_PA_05 // byte tclk_scl
                   ,
                   WM_IO_PA_06 // byte tcs_sda
                   ,
                   NO_GPIO_PIN // byte tdin
                   ,
                   NO_GPIO_PIN // byte dout
                   ,
                   WM_IO_PA_09 // byte irq
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

  if (TOUCH_ORIENTATION == PORTRAIT)
    cals = ((int32_t)(dispx - 1) << 12) + (dispy - 1);
  else
    cals = ((int32_t)(dispy - 1) << 12) + (dispx - 1);

  if (TOUCH_ORIENTATION == LANDSCAPE)
    px = abs ((((float)(rx[2] + rx[4] + rx[7]) / 3)
               - ((float)(rx[0] + rx[3] + rx[5]) / 3))
              / (dispy - 20)); 
  else
    px = abs ((((float)(rx[5] + rx[6] + rx[7]) / 3)
               - ((float)(rx[0] + rx[1] + rx[2]) / 3))
              / (dispy - 20)); 

  if (TOUCH_ORIENTATION == LANDSCAPE)
    {
      clx = (((rx[0] + rx[3] + rx[5]) / 3)); 
      crx = (((rx[2] + rx[4] + rx[7]) / 3)); 
    }
  else
    {
      clx = (((rx[0] + rx[1] + rx[2]) / 3)); 
      crx = (((rx[5] + rx[6] + rx[7]) / 3)); 
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

  if (TOUCH_ORIENTATION == LANDSCAPE)
    py = abs ((((float)(ry[5] + ry[6] + ry[7]) / 3)
               - ((float)(ry[0] + ry[1] + ry[2]) / 3))
              / (dispx - 20)); 
  else
    py = abs ((((float)(ry[0] + ry[3] + ry[5]) / 3)
               - ((float)(ry[2] + ry[4] + ry[7]) / 3))
              / (dispx - 20)); 

  if (TOUCH_ORIENTATION == LANDSCAPE)
    {
      cty = (((ry[5] + ry[6] + ry[7]) / 3)); 
      cby = (((ry[0] + ry[1] + ry[2]) / 3)); 
    }
  else
    {
      cty = (((ry[0] + ry[3] + ry[5]) / 3)); 
      cby = (((ry[2] + ry[4] + ry[7]) / 3)); 
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

  calx = ((int32_t)(clx) << 14) + (int32_t)(crx);
  caly = ((int32_t)(cty) << 14) + (int32_t)(cby);


//  if (TOUCH_ORIENTATION == LANDSCAPE)
//    cals = cals + (1L << 31);

  done ();


// LANDSCAPE = URTouch_set_calibrate: calx=3CC40D9, caly=2BCE3A, cals=EF13F
// PORTRAIT  = URTouch_set_calibrate: calx=3C00111, caly=200E6C, cals=EF13F


 URTouch_set_calibrate (calx, caly, cals);

  int x = 0, y = 0;
  // UTFT_clrScr();
  while (1)
    { //

      if (URTouch_dataAvailable ())
        {
          URTouch_read ();
          x = URTouch_getX ();
          y = URTouch_getY ();
          //x = UTFT_getDisplayXSize () - x;
          //y = UTFT_getDisplayYSize () - y;
          printf ("touch X=%.3d Y=%.3d\n", x, y);
          if (x >= 0 && y >= 0)
            {
              // char mesg[50];
              // sprintf (mesg, "X=%.3d Y=%.3d", x, y);
              // UTFT_print (mesg, CENTER, 10, 0);
              UTFT_setColor2 (VGA_BLUE); // 800x480
              UTFT_fillCircle (x, y, 2); // Рисуем закрашенную окружность
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
