/*
  URTouch.h - Arduino/chipKit library support for Color TFT LCD Touch screens
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

#ifndef URTOUCH_h
#define URTOUCH_h

#ifdef __cplusplus
extern "C"
{
#endif

#include "wm_hostspi.h"
#include "wm_type_def.h"

#include <stdio.h>
#include <stdlib.h>

#include "wm_gpio.h"

#include "FT6236.h"


  typedef u8 byte;
  typedef u16 word;

#define URTOUCH_VERSION 202

#define NO_GPIO_PIN 255

#define PORTRAIT 0
#define LANDSCAPE 1

#define PREC_LOW 1
#define PREC_MEDIUM 2
#define PREC_HI 3
#define PREC_EXTREME 4

  extern int16_t URTouch_TP_X, URTouch_TP_Y;
  extern volatile bool URTouch_flag_touch_isr;

  typedef enum sensor_type
  {
    TS_XPT2046 = 0,
    TS_FT6236 = 1
  } _sensor_type;

  void URTouch_URTouch (enum sensor_type sensor, byte tclk_scl, byte tcs_sda,
                        byte din_thresh, byte dout_none, byte irq_irq);
  void URTouch_InitTouch (byte orientation); // byte orientation = LANDSCAPE
  void URTouch_read ();
  bool URTouch_dataAvailable ();
  int16_t URTouch_getX ();
  int16_t URTouch_getY ();
  void URTouch_setPrecision (byte precision);
  void URTouch_calibrateRead ();
  void URTouch_set_calibrate (uint32_t calx, uint32_t caly, uint32_t cals);

#ifdef __cplusplus
}
#endif

#endif /* end URTOUCH_h */
