/*
  URTouch.cpp - Arduino/chipKit library support for Color TFT LCD Touch screens
  Copyright (C)2018 Rinky-Dink Electronics, Henning Karlsen. All right reserved

  Basic functionality of this library are based on the demo-code provided by
  ITead studio.

  You can find the latest version of the library at
  http://www.RinkyDinkElectronics.com/

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the
  examples and tools supplied with the library.
*/

#include "URTouch.h"
#include "FT6236.h"
#include "URTouchCD.h"
#include <mod1/UTFT.h>

#include "hardware/sky/HW_SKY_defines.h"

int16_t URTouch_TP_X, URTouch_TP_Y;
int16_t URTouch_TP_X2, URTouch_TP_Y2;

static regtype *P_CLK, *P_CS, *P_DIN, *P_DOUT, *P_IRQ;
static regsize B_CLK, B_CS, B_DIN, B_DOUT, B_IRQ;
static byte T_CLK, T_CS, T_DIN, T_DOUT, T_IRQ;
static enum sensor_type T_SENSOR;
static int32_t _default_orientation;
static byte orient;
static byte prec;
// static byte	display_model;
static int32_t disp_x_size, disp_y_size; //, default_orientation;
static int32_t touch_x_left, touch_x_right, touch_y_top, touch_y_bottom;

static void URTouch_touch_WriteData (byte data);
static word URTouch_touch_ReadData ();

#include "hardware/sky/HW_W801_p.h"

#include "hardware/sky/HW_SKY.h"

#define URT_SERIAL_DEBUG

void
URTouch_URTouch (enum sensor_type sensor, byte tclk_scl, byte tcs_sda,
                 byte din_thresh, byte dout_none, byte irq_irq)
{
  T_SENSOR = sensor;
  T_CLK = tclk_scl;
  T_CS = tcs_sda;
  T_DIN = din_thresh;
  T_DOUT = dout_none;
  T_IRQ = irq_irq;
}

void
URTouch_set_calibrate (uint32_t calx, uint32_t caly, uint32_t cals)
{
#ifdef URT_SERIAL_DEBUG
  printf ("URTouch_set_calibrate: calx=%X, caly=%X, cals=%X\n", calx, caly,
          cals);
#endif
  _default_orientation = cals >> 31;
  touch_x_left = (calx >> 14) & 0x3FFF;
  touch_x_right = calx & 0x3FFF;
  touch_y_top = (caly >> 14) & 0x3FFF;
  touch_y_bottom = caly & 0x3FFF;
  disp_x_size = (cals >> 12) & 0x0FFF;
  disp_y_size = cals & 0x0FFF;
#ifdef URT_SERIAL_DEBUG
  printf (" _default_orientation=%X\n", _default_orientation);
  printf (" touch_x_left=%X=%d, touch_x_right=%X=%d\n", touch_x_left,
          touch_x_left, touch_x_right, touch_x_right);
  printf (" touch_y_top=%X=%d, touch_y_bottom=%X=%d\n", touch_y_top,
          touch_y_top, touch_y_bottom, touch_y_bottom);
  printf (" disp_x_size=%X=%d, disp_y_size=%X=%d\n", disp_x_size, disp_x_size,
          disp_y_size, disp_y_size);
#endif
}

volatile bool URTouch_flag_touch_isr = false;
static void
isr_callback (void *context)
{
  u16 ret = tls_get_gpio_irq_status (T_IRQ);
  if (ret)
    {
      tls_clr_gpio_irq_status (T_IRQ);
      URTouch_flag_touch_isr = true;
      // if (i_dreb_SW == 0) // защита от ддребезга контактов для кнопки
      //  {
      //    i_dreb_SW = 1;
      //  }
    }
}

void
URTouch_InitTouch (byte orientation)
{
  orient = orientation;
  URTouch_set_calibrate (CAL_X, CAL_Y, CAL_S);
  prec = 10;

  P_CLK = portOutputRegister (digitalPinToPort (T_CLK));
  B_CLK = digitalPinToBitMask (T_CLK);
  P_CS = portOutputRegister (digitalPinToPort (T_CS));
  B_CS = digitalPinToBitMask (T_CS);
  P_DIN = portOutputRegister (digitalPinToPort (T_DIN));
  B_DIN = digitalPinToBitMask (T_DIN);
  P_DOUT = portInputRegister (digitalPinToPort (T_DOUT));
  B_DOUT = digitalPinToBitMask (T_DOUT);
  P_IRQ = portInputRegister (digitalPinToPort (T_IRQ));
  B_IRQ = digitalPinToBitMask (T_IRQ);

  switch ((int)T_SENSOR)
    {
    case TS_XPT2046:
      {
        pinMode (T_CLK, OUTPUT);
        pinMode (T_CS, OUTPUT);
        pinMode (T_DIN, OUTPUT);
        pinMode (T_DOUT, INPUT);

        sbi (P_CS, B_CS);
        sbi (P_CLK, B_CLK);
        sbi (P_DIN, B_DIN);
        sbi (P_IRQ, B_IRQ);
      };
      break;
    case TS_FT6236:
      {
        FT6236_begin (T_DIN, T_CS, T_CLK);
        // boolean FT6236_begin (uint8_t thresh, int8_t sda, int8_t scl);
        disp_x_size = UTFT_getDisplayXSize ();
        disp_y_size = UTFT_getDisplayYSize ();
      };
      break;
    }

  URTouch_flag_touch_isr = false;
  tls_gpio_cfg (T_IRQ, WM_GPIO_DIR_INPUT,
                WM_GPIO_ATTR_PULLHIGH); // WM_GPIO_ATTR_FLOATING
  tls_gpio_isr_register (T_IRQ, isr_callback, NULL);
  tls_gpio_irq_enable (T_IRQ, WM_GPIO_IRQ_TRIG_FALLING_EDGE);
}

void
URTouch_read ()
{
  tls_gpio_irq_disable (T_IRQ);
  URTouch_flag_touch_isr = false;

  switch ((int)T_SENSOR)
    {
    case TS_XPT2046:
      {
        uint32_t tx = 0, temp_x = 0;
        uint32_t ty = 0, temp_y = 0;
        uint32_t minx = 99999, maxx = 0;
        uint32_t miny = 99999, maxy = 0;
        int datacount = 0;

        cbi (P_CS, B_CS);

        // pinMode (T_IRQ, INPUT);
        for (int i = 0; i < prec; i++)
          {
            if (!rbi (P_IRQ, B_IRQ))
              {
                URTouch_touch_WriteData (0x90);
                pulse_high (P_CLK, B_CLK);
                temp_x = URTouch_touch_ReadData ();

                if (!rbi (P_IRQ, B_IRQ))
                  {
                    URTouch_touch_WriteData (0xD0);
                    pulse_high (P_CLK, B_CLK);
                    temp_y = URTouch_touch_ReadData ();

                    if ((temp_x > 0) && (temp_x < 4096) && (temp_y > 0)
                        && (temp_y < 4096))
                      {
                        tx += temp_x;
                        ty += temp_y;
                        if (prec > 5)
                          {
                            if (temp_x < minx)
                              minx = temp_x;
                            if (temp_x > maxx)
                              maxx = temp_x;
                            if (temp_y < miny)
                              miny = temp_y;
                            if (temp_y > maxy)
                              maxy = temp_y;
                          }
                        datacount++;
                      }
                  }
              }
          }
        // pinMode (T_IRQ, OUTPUT);

        if (prec > 5)
          {
            tx = tx - (minx + maxx);
            ty = ty - (miny + maxy);
            datacount -= 2;
          }

        sbi (P_CS, B_CS);
        if ((datacount == (prec - 2)) || (datacount == PREC_LOW))
          {
            if (orient == _default_orientation)
              {
                URTouch_TP_X = ty / datacount;
                URTouch_TP_Y = tx / datacount;
              }
            else
              {
                URTouch_TP_X = tx / datacount;
                URTouch_TP_Y = ty / datacount;
              }
          }
        else
          {
            URTouch_TP_X = -1;
            URTouch_TP_Y = -1;
          }
      };
      break;
    case TS_FT6236:
      {
        TS_Point p = FT6236_getPoint ();
        if (orient == LANDSCAPE)
          {
            if (p.z == 1)
              URTouch_TP_X = disp_x_size - p.y;
            else
              URTouch_TP_X = 0;
            URTouch_TP_Y = p.x;
            if (p.z2 == 1)
              URTouch_TP_X2 = disp_x_size - p.y2;
            else
              URTouch_TP_X2 = 0;
            URTouch_TP_Y2 = p.x2;
          }
        else
          {
            URTouch_TP_X = p.x;
            URTouch_TP_Y = p.y;
            URTouch_TP_X2 = p.x2;
            URTouch_TP_Y2 = p.y2;
          }
      };
      break;
    }
  // printf ("X=%d,Y=%d  -  x=%d,y=%d\n", URTouch_TP_X,
  // URTouch_TP_Y,disp_x_size,disp_y_size);

  tls_gpio_irq_enable (T_IRQ, WM_GPIO_IRQ_TRIG_FALLING_EDGE);
}

bool
URTouch_dataAvailable ()
{
  tls_gpio_irq_disable (T_IRQ);
  bool avail = URTouch_flag_touch_isr;
  URTouch_flag_touch_isr = false;

  if (!avail)
    {
      switch ((int)T_SENSOR)
        {
        case TS_XPT2046:
          {
            // pinMode (T_IRQ, INPUT);
            avail = !(rbi (P_IRQ, B_IRQ));
            // pinMode (T_IRQ, OUTPUT);
          };
          break;
        case TS_FT6236:
          {
            avail = FT6236_touched ();
          };
          break;
        }
    }

  tls_gpio_irq_enable (T_IRQ, WM_GPIO_IRQ_TRIG_FALLING_EDGE);
  return avail;
}

int16_t
URTouch_getX ()
{
  int32_t c = -1;

  if ((URTouch_TP_X == -1) || (URTouch_TP_Y == -1))
    return c;

  switch ((int)T_SENSOR)
    {
    case TS_XPT2046:
      {

        if (orient == _default_orientation)
          {
            c = (int32_t) ((int32_t) (URTouch_TP_X - touch_x_left)
                           * (disp_x_size))
                / (int32_t) (touch_x_right - touch_x_left);
            if (c < 0)
              c = 0;
            if (c > disp_x_size)
              c = disp_x_size;
          }
        else
          {
            if (_default_orientation == PORTRAIT)
              c = (int32_t) ((int32_t) (URTouch_TP_X - touch_y_top)
                             * (-disp_y_size))
                      / (int32_t) (touch_y_bottom - touch_y_top)
                  + (int32_t) (disp_y_size);
            else
              c = (int32_t) ((int32_t) (URTouch_TP_X - touch_y_top)
                             * (disp_y_size))
                  / (int32_t) (touch_y_bottom - touch_y_top);
            if (c < 0)
              c = 0;
            if (c > disp_y_size)
              c = disp_y_size;
          }
      };
      break;
    case TS_FT6236:
      {
        c = URTouch_TP_X;
      };
      break;
    }

  return c;
}

int16_t
URTouch_getY ()
{
  int32_t c = -1;

  if ((URTouch_TP_X == -1) || (URTouch_TP_Y == -1))
    return c;

  switch ((int)T_SENSOR)
    {
    case TS_XPT2046:
      {

        if (orient == _default_orientation)
          {
            c = (int32_t) ((int32_t) (URTouch_TP_Y - touch_y_top)
                           * (disp_y_size))
                / (int32_t) (touch_y_bottom - touch_y_top);
            if (c < 0)
              c = 0;
            if (c > disp_y_size)
              c = disp_y_size;
          }
        else
          {
            if (_default_orientation == PORTRAIT)
              c = (int32_t) ((int32_t) (URTouch_TP_Y - touch_x_left)
                             * (disp_x_size))
                  / (int32_t) (touch_x_right - touch_x_left);
            else
              c = (int32_t) ((int32_t) (URTouch_TP_Y - touch_x_left)
                             * (-disp_x_size))
                      / (int32_t) (touch_x_right - touch_x_left)
                  + (int32_t) (disp_x_size);
            if (c < 0)
              c = 0;
            if (c > disp_x_size)
              c = disp_x_size;
          }
      };
      break;
    case TS_FT6236:
      {
        c = URTouch_TP_Y;
      };
      break;
    }
  return c;
}

void
URTouch_setPrecision (byte precision)
{
  switch (precision)
    {
    case PREC_LOW:
      prec = 1; // DO NOT CHANGE!
      break;
    case PREC_MEDIUM:
      prec = 12; // Iterations + 2
      break;
    case PREC_HI:
      prec = 27; // Iterations + 2
      break;
    case PREC_EXTREME:
      prec = 102; // Iterations + 2
      break;
    default:
      prec = 12; // Iterations + 2
      break;
    }
}

void
URTouch_calibrateRead ()
{
  tls_gpio_irq_disable (T_IRQ);
  URTouch_flag_touch_isr = false;

  word tx = 0;
  word ty = 0;

  switch ((int)T_SENSOR)
    {
    case TS_XPT2046:
      {
        cbi (P_CS, B_CS);

        URTouch_touch_WriteData (0x90);
        pulse_high (P_CLK, B_CLK);
        tx = URTouch_touch_ReadData ();

        URTouch_touch_WriteData (0xD0);
        pulse_high (P_CLK, B_CLK);
        ty = URTouch_touch_ReadData ();

        sbi (P_CS, B_CS);
      };
      break;
    case TS_FT6236:
      {
        TS_Point p = FT6236_getPoint ();
        ty = p.y;
        tx = p.x;
      };
      break;
    }

  URTouch_TP_X = ty;
  URTouch_TP_Y = tx;

  tls_gpio_irq_enable (T_IRQ, WM_GPIO_IRQ_TRIG_FALLING_EDGE);

  //#ifdef URT_SERIAL_DEBUG
  //     printf ("URTouch_calibrateRead: tx=%X, ty=%X \n", tx,ty);
  //#endif
}
