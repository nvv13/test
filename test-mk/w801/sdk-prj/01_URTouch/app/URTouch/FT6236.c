/*
This is a library for the FT6236 touchscreen controller by FocalTech.

The FT6236 and FT6236u work the same way.

A lot of this library is originally written by Limor Fried/Ladyada.

Because Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

@section author Author
Written by Limor Fried/Ladyada for Adafruit Industries.

@section license License
MIT license, all text above must be included in any redistribution
*/

#include "FT6236.h"

#include "wm_gpio_afsel.h"
//#include "wm_hostspi.h"

#include "SoftWire.h"

static void FT6236_writeRegister8 (uint8_t reg, uint8_t val);
static uint8_t FT6236_readRegister8 (uint8_t reg);
static void FT6236_readData (void);
static void FT6236_debug (void);

static uint8_t touches;
static uint16_t touchX[2], touchY[2], touchID[2];

/* Start I2C and check if the FT6236 is found. */
boolean
FT6236_begin (uint8_t thresh, int8_t sda, int8_t scl)
{

  touches = 0;

  IIC_Init (scl, sda);

  FT6236_debug ();

  // Adjust threshold
  FT6236_writeRegister8 (FT6236_REG_THRESHHOLD, thresh);

  // Check if our chip has the correct Vendor ID
  if (FT6236_readRegister8 (FT6236_REG_VENDID) != FT6236_VENDID)
    {
      return false;
    }
  // Check if our chip has the correct Chip ID.
  uint8_t id = FT6236_readRegister8 (FT6236_REG_CHIPID);
  if ((id != FT6236_CHIPID) && (id != FT6236U_CHIPID) && (id != FT6206_CHIPID))
    {
      return false;
    }

  return true;
}

/* Returns the number of touches */
uint8_t
FT6236_touched (void)
{
  uint8_t n = FT6236_readRegister8 (FT6236_REG_NUMTOUCHES);
  if (n > 2)
    {
      n = 0;
    }
  return n;
}

static TS_Point p1;
/* Get a touch point */
TS_Point
FT6236_getPoint (void)
{
  FT6236_readData ();
  if ((touches == 0)) // || (n > 1))
    {
      p1.x = 0;
      p1.y = 0;
      p1.z = 0;
      return p1;
    }
  else
    {
      p1.x = touchX[0];
      p1.y = touchY[0];
      p1.z = 1;
      if (touches == 2)
        {
          p1.x2 = touchX[1];
          p1.y2 = touchY[1];
          p1.z2 = 1;
        }
      else
        {
          p1.x2 = 0;
          p1.y2 = 0;
          p1.z2 = 0;
        }
      return p1;
    }
}

void
FT6236_readData (void)
{

  uint8_t i2cdat[16];

  Wire_read_buf (FT6236_ADDR,0, i2cdat, 16);

  touches = i2cdat[0x02];
  if ((touches > 2) || (touches == 0))
    {
      touches = 0;
    }

  for (uint8_t i = 0; i < 2; i++)
    {
      touchX[i] = i2cdat[0x03 + i * 6] & 0x0F;
      touchX[i] <<= 8;
      touchX[i] |= i2cdat[0x04 + i * 6];
      touchY[i] = i2cdat[0x05 + i * 6] & 0x0F;
      touchY[i] <<= 8;
      touchY[i] |= i2cdat[0x06 + i * 6];
      touchID[i] = i2cdat[0x05 + i * 6] >> 4;
    }
}

/* Reading a byte from a register */
uint8_t
FT6236_readRegister8 (uint8_t reg)
{
  uint8_t x = 0;
  Wire_read_buf (FT6236_ADDR,reg, &x, 1);
  return x;
}

/* Writing a byte to a register */
void
FT6236_writeRegister8 (uint8_t reg, uint8_t val)
{
  Wire_write_buf(FT6236_ADDR,reg,&val,1);
}

/* Debug */
void
FT6236_debug (void)
{
  printf ("Vend ID: 0x%02X \n", FT6236_readRegister8 (FT6236_REG_VENDID));
  printf ("Chip ID: 0x%02X \n", FT6236_readRegister8 (FT6236_REG_CHIPID));
  printf ("Firm V: %d\n", FT6236_readRegister8 (FT6236_REG_FIRMVERS));
  printf ("Point Rate Hz: %d\n", FT6236_readRegister8 (FT6236_REG_POINTRATE));
  printf ("Thresh: %d\n", FT6236_readRegister8 (FT6236_REG_THRESHHOLD));
}
