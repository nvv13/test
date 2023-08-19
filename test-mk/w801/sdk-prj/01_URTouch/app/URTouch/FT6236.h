/*!
 * FT6236.h
 */

#ifndef FT6236_H
#define FT6236_H

#define FT6236_ADDR 0x38           // I2C address
#define FT6236_G_FT5201ID 0xA8     // FocalTech's panel ID
#define FT6236_REG_NUMTOUCHES 0x02 // Number of touch points

#define FT6236_NUM_X 0x33 // Touch X position
#define FT6236_NUM_Y 0x34 // Touch Y position

#define FT6236_REG_MODE 0x00        // Device mode, either WORKING or FACTORY
#define FT6236_REG_CALIBRATE 0x02   // Calibrate mode
#define FT6236_REG_WORKMODE 0x00    // Work mode
#define FT6236_REG_FACTORYMODE 0x40 // Factory mode
#define FT6236_REG_THRESHHOLD 0x80  // Threshold for touch detection
#define FT6236_REG_POINTRATE 0x88   // Point rate
#define FT6236_REG_FIRMVERS 0xA6    // Firmware version
#define FT6236_REG_CHIPID 0xA3      // Chip selecting
#define FT6236_REG_VENDID 0xA8      // FocalTech's panel ID

#define FT6236_VENDID 0x11  // FocalTech's panel ID
#define FT6206_CHIPID 0x06  // FT6206 ID
#define FT6236_CHIPID 0x36  // FT6236 ID
#define FT6236U_CHIPID 0x64 // FT6236U ID

#define FT6236_DEFAULT_THRESHOLD 128 // Default threshold for touch detection

#include "wm_type_def.h"

#include "wm_io.h"

#include <stdio.h>
#include <stdlib.h>

#include "wm_gpio.h"

typedef s16 int16_t;
typedef bool boolean;
typedef s8 int8_t;
typedef u8 byte;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;

  int16_t x2;
  int16_t y2;
  int16_t z2;
} TS_Point;

boolean FT6236_begin (uint8_t thresh, int8_t sda, int8_t scl);
// boolean FT6236_begin(uint8_t thresh = FT6236_DEFAULT_THRESHOLD, int8_t sda =
// -1, int8_t scl = -1);
uint8_t FT6236_touched (void);
TS_Point FT6236_getPoint (void);

#endif
