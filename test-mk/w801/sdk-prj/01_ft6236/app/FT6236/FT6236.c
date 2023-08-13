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

#include "wm_i2c.h"


static void Wire_begin(enum tls_io_name i2c_scl,enum tls_io_name i2c_sda)
{
      wm_i2c_scl_config (i2c_scl);
      wm_i2c_sda_config (i2c_sda);
      tls_i2c_init (100000);

}

static u8
Wire_beginTransmission (u8 AddresI2C)
{
  tls_reg_write32 (HR_I2C_TX_RX,
                   (AddresI2C << 1)); // заносим в регистр данных адрес слейва

  tls_reg_write32 (
      HR_I2C_CR_SR,
      I2C_CR_STA | I2C_CR_WR); // включаем модуль на передачу и выдаем START

  while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_TIP)
    ; // ждем окончания передачи

  if (tls_reg_read32 (HR_I2C_CR_SR)
      & I2C_SR_NAK) // если по окончанию передачи байта слейв не ответил
    {
      tls_reg_write32 (HR_I2C_CR_SR, I2C_CR_STO); // останавливаем обмен,
      while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_BUSY)
        ; // ожидаем освобождения шины
      printf ("wire_beginTransmission error ASK AddresI2C= 0x%.2X \n",
              AddresI2C);
      return 1; // возвращаем код ошибки "1"
    }           // если есть ответ от слейва

  return 0; // возвращаем "0" - передача успешна
}


static u8
Wire_write_buf (u8 *cmdI2C, u8 u8_Len)
{
  for (u8 u8_index = 0; u8_index < u8_Len; u8_index++)
    {
      u8 data = cmdI2C[u8_index];

      tls_reg_write32 (HR_I2C_TX_RX,
                       data); // заносим в регистр данных байт на отправку
      if (u8_index == (u8_Len - 1))
        {
          tls_reg_write32 (HR_I2C_CR_SR, // I2C_CR_NAK |
                           I2C_CR_WR
                               | I2C_CR_STO); // передаем байт и по окончании
                                              // передачи - STOP
#ifdef LCD_SERIAL_DEBUG
          printf ("i2c wire_write I2C_CR_STO data= 0x%.2X \n", data);
#endif
        }
      else
        {
          tls_reg_write32 (
              HR_I2C_CR_SR, // I2C_CR_NAK |
              I2C_CR_WR); // передаем байт и по окончании передачи - STOP
#ifdef LCD_SERIAL_DEBUG
          printf ("i2c wire_write data= 0x%.2X \n", data);
#endif
        }

      // while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_BUSY)
      //  ;       // ожидаем освобождения шины
      while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_TIP)
        ; // ждем окончания передачи

      if (tls_reg_read32 (HR_I2C_CR_SR)
          & I2C_SR_NAK) // если по окончанию передачи байта слейв не ответил
        {
          tls_reg_write32 (HR_I2C_CR_SR, I2C_CR_STO); // останавливаем обмен,
          while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_BUSY)
            ; // ожидаем освобождения шины
          printf ("i2c wire_write error ASK data= 0x%.2X \n", data);
          return 1; // возвращаем код ошибки "1"
        }           // если есть ответ от слейва
    }
  return 0; // возвращаем "0" - передача успешна
}
static u8
Wire_write (u8 cmdI2C)
{
return Wire_write_buf (&cmdI2C, 1);
}

static u8 Wire_requestFrom(u8 AddresI2C, u8 len)
{
return 0;
}

u8 Wire_read(void)
{
 return 0;
}

/*                                                             	
Example
#include <Wire.h>

void setup() {
  Wire.begin();             // Join I2C bus (address is optional for controller device)
  Serial.begin(9600);       // Start serial for output
}

void loop() {
    Wire.requestFrom(2, 6);    // Request 6 bytes from slave device number two

    // Slave may send less than requested
    while(Wire.available()) {
        char c = Wire.read();    // Receive a byte as character
        Serial.print(c);         // Print the character
    }

    delay(500);
}
*/
static u8
Wire_endTransmission (void)
{
  if (tls_reg_read32 (HR_I2C_CR_SR)
      & I2C_SR_NAK) // если по окончанию передачи байта слейв не ответил
    {
      tls_reg_write32 (HR_I2C_CR_SR, I2C_CR_STO); // останавливаем обмен,
      while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_BUSY)
        ; // ожидаем освобождения шины
      printf ("wire_endTransmission error ASK\n");
      return 1; // возвращаем код ошибки "1"
    }           // если есть ответ от слейва

  return 0; // возвращаем "0" - передача успешна
}



static  void    FT6236_writeRegister8(uint8_t reg, uint8_t val);
static  uint8_t FT6236_readRegister8(uint8_t reg);
static  void    FT6236_readData(void);

static uint8_t touches;
static uint16_t touchX[2], touchY[2], touchID[2];


/* Start I2C and check if the FT6236 is found. */
boolean FT6236_begin(uint8_t thresh, int8_t sda, int8_t scl)
{

    touches = 0;

    Wire_begin(sda, scl);

    // Adjust threshold
    FT6236_writeRegister8(FT6236_REG_THRESHHOLD, thresh);

    //Check if our chip has the correct Vendor ID
    if (FT6236_readRegister8(FT6236_REG_VENDID) != FT6236_VENDID)
    {
        return false;
    }
    //Check if our chip has the correct Chip ID.
    uint8_t id = FT6236_readRegister8(FT6236_REG_CHIPID);
    if ((id != FT6236_CHIPID) && (id != FT6236U_CHIPID) &&
      (id != FT6206_CHIPID))
    {
        return false;
    }

    return true;
}

/* Returns the number of touches */
uint8_t FT6236_touched(void)
{
    uint8_t n = FT6236_readRegister8(FT6236_REG_NUMTOUCHES);
    if (n > 2)
    {
        n = 0;
    }
    return n;
}

static TS_Point p1;
/* Get a touch point */
TS_Point FT6236_getPoint(uint8_t n)
{
    FT6236_readData();
    if ((touches == 0) || (n > 1))
    {
        p1.x=0;
        p1.y=0;
        p1.z=0;
        return p1;
    }
    else
    {
        p1.x=touchX[n];
        p1.y=touchY[n];
        p1.z=1;
        return p1;
    }
}

void FT6236_readData(void)
{

    uint8_t i2cdat[16];
    Wire_beginTransmission(FT6236_ADDR);
    Wire_write((byte)0);
    Wire_endTransmission();

    Wire_requestFrom((byte)FT6236_ADDR, (byte)16);
    for (uint8_t i = 0; i < 16; i++)
        i2cdat[i] = Wire_read();

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
uint8_t FT6236_readRegister8(uint8_t reg)
{
    uint8_t x;

    Wire_beginTransmission(FT6236_ADDR);
    Wire_write((byte)reg);
    Wire_endTransmission();

    Wire_requestFrom((byte)FT6236_ADDR, (byte)1);
    x = Wire_read();

    return x;
}

/* Writing a byte to a register */
void FT6236_writeRegister8(uint8_t reg, uint8_t val)
{

    Wire_beginTransmission(FT6236_ADDR);
    Wire_write((byte)reg);
    Wire_write((byte)val);
    Wire_endTransmission();
}

/* Debug */
void FT6236_debug(void)
{
    printf("Vend ID: 0x%02X \n",FT6236_readRegister8(FT6236_REG_VENDID));
    printf("Chip ID: 0x%02X \n",FT6236_readRegister8(FT6236_REG_CHIPID));
    printf("Firm V: %d\n",FT6236_readRegister8(FT6236_REG_FIRMVERS));
    printf("Point Rate Hz: %d\n",FT6236_readRegister8(FT6236_REG_POINTRATE));
    printf("Thresh: %d\n",FT6236_readRegister8(FT6236_REG_THRESHHOLD));
}

