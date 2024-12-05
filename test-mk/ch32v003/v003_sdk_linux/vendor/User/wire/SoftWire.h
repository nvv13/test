/*
Soft IIC источник
https://github.com/ProYCS/ft6336_driver_for_stm32.git
*/
//**********************************************************************************************
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "endian.h"

/*

 mc - Milliseconds 10^-3

 µs - Microseconds 10^-6
 us - Microseconds 10^-6

 ns - Nanoseconds  10^-9

 1000 ns = 1 µs OR 1 us

*/

//static void _n_delay_us (uint32_t us);
//static uint8_t p_us = 0;

/**
 * @brief     interface delay us (Microseconds 10^-6)
 * @param[in] us
 * @note      none
 */
//static void
//_n_delay_us (uint32_t us)
//{
//  uint32_t i = SysTick->CNT + (uint32_t)us * p_us;
//  SysTick->CTLR |=(1 << 0);
//  while (SysTick->CNT < i)
//    ;
//}

static GPIOSpeed_TypeDef GPIO_Speed = GPIO_Speed_50MHz;

static GPIO_TypeDef *port_i2c_scl;
static u16 pin_i2c_scl;

static GPIO_TypeDef *port_i2c_sda;
static u16 pin_i2c_sda;

#define IIC_SCL(n)                               \
  (n ? GPIO_SetBits(port_i2c_sda, pin_i2c_sda)   \
     : GPIO_ResetBits(port_i2c_sda, pin_i2c_sda))
#define IIC_SDA(n)                               \
  (n ? GPIO_SetBits(port_i2c_sda, pin_i2c_sda)   \
     : GPIO_ResetBits(port_i2c_sda, pin_i2c_sda))
#define READ_SDA GPIO_ReadInputDataBit (port_i2c_sda, pin_i2c_sda)
#define delay_us(n) Delay_Us(n)

static void
SWire_init (i2c_t dev)
{
  //p_us = SystemCoreClock / 8000000;

  GPIO_TypeDef *port = GPIOA;
  u8 GPIO_Pin = 0;
  switch (dev->i2c_scl)
    {
    case PA_00:
      {
        GPIO_Pin = GPIO_Pin_0;
        port = GPIOA;
      };
      break;
    case PA_01:
      {
        GPIO_Pin = GPIO_Pin_1;
        port = GPIOA;
      };
      break;
    case PA_02:
      {
        GPIO_Pin = GPIO_Pin_2;
        port = GPIOA;
      };
      break;
    case PA_03:
      {
        GPIO_Pin = GPIO_Pin_3;
        port = GPIOA;
      };
      break;
    case PA_04:
      {
        GPIO_Pin = GPIO_Pin_4;
        port = GPIOA;
      };
      break;
    case PA_05:
      {
        GPIO_Pin = GPIO_Pin_5;
        port = GPIOA;
      };
      break;
    case PA_06:
      {
        GPIO_Pin = GPIO_Pin_6;
        port = GPIOA;
      };
      break;
    case PA_07:
      {
        GPIO_Pin = GPIO_Pin_7;
        port = GPIOA;
      };
      break;
    case PC_00:
      {
        GPIO_Pin = GPIO_Pin_0;
        port = GPIOC;
      };
      break;
    case PC_01:
      {
        GPIO_Pin = GPIO_Pin_1;
        port = GPIOC;
      };
      break;
    case PC_02:
      {
        GPIO_Pin = GPIO_Pin_2;
        port = GPIOC;
      };
      break;
    case PC_03:
      {
        GPIO_Pin = GPIO_Pin_3;
        port = GPIOC;
      };
      break;
    case PC_04:
      {
        GPIO_Pin = GPIO_Pin_4;
        port = GPIOC;
      };
      break;
    case PC_05:
      {
        GPIO_Pin = GPIO_Pin_5;
        port = GPIOC;
      };
      break;
    case PC_06:
      {
        GPIO_Pin = GPIO_Pin_6;
        port = GPIOC;
      };
      break;
    case PC_07:
      {
        GPIO_Pin = GPIO_Pin_7;
        port = GPIOC;
      };
      break;
    case PD_00:
      {
        GPIO_Pin = GPIO_Pin_0;
        port = GPIOD;
      };
      break;
    case PD_01:
      {
        GPIO_Pin = GPIO_Pin_1;
        port = GPIOD;
      };
      break;
    case PD_02:
      {
        GPIO_Pin = GPIO_Pin_2;
        port = GPIOD;
      };
      break;
    case PD_03:
      {
        GPIO_Pin = GPIO_Pin_3;
        port = GPIOD;
      };
      break;
    case PD_04:
      {
        GPIO_Pin = GPIO_Pin_4;
        port = GPIOD;
      };
      break;
    case PD_05:
      {
        GPIO_Pin = GPIO_Pin_5;
        port = GPIOD;
      };
      break;
    case PD_06:
      {
        GPIO_Pin = GPIO_Pin_6;
        port = GPIOD;
      };
      break;
    case PD_07:
      {
        GPIO_Pin = GPIO_Pin_7;
        port = GPIOD;
      };
      break;
    }
  port_i2c_scl = port;
  pin_i2c_scl = GPIO_Pin;

  switch (dev->i2c_sda)
    {
    case PA_00:
      {
        GPIO_Pin = GPIO_Pin_0;
        port = GPIOA;
      };
      break;
    case PA_01:
      {
        GPIO_Pin = GPIO_Pin_1;
        port = GPIOA;
      };
      break;
    case PA_02:
      {
        GPIO_Pin = GPIO_Pin_2;
        port = GPIOA;
      };
      break;
    case PA_03:
      {
        GPIO_Pin = GPIO_Pin_3;
        port = GPIOA;
      };
      break;
    case PA_04:
      {
        GPIO_Pin = GPIO_Pin_4;
        port = GPIOA;
      };
      break;
    case PA_05:
      {
        GPIO_Pin = GPIO_Pin_5;
        port = GPIOA;
      };
      break;
    case PA_06:
      {
        GPIO_Pin = GPIO_Pin_6;
        port = GPIOA;
      };
      break;
    case PA_07:
      {
        GPIO_Pin = GPIO_Pin_7;
        port = GPIOA;
      };
      break;
    case PC_00:
      {
        GPIO_Pin = GPIO_Pin_0;
        port = GPIOC;
      };
      break;
    case PC_01:
      {
        GPIO_Pin = GPIO_Pin_1;
        port = GPIOC;
      };
      break;
    case PC_02:
      {
        GPIO_Pin = GPIO_Pin_2;
        port = GPIOC;
      };
      break;
    case PC_03:
      {
        GPIO_Pin = GPIO_Pin_3;
        port = GPIOC;
      };
      break;
    case PC_04:
      {
        GPIO_Pin = GPIO_Pin_4;
        port = GPIOC;
      };
      break;
    case PC_05:
      {
        GPIO_Pin = GPIO_Pin_5;
        port = GPIOC;
      };
      break;
    case PC_06:
      {
        GPIO_Pin = GPIO_Pin_6;
        port = GPIOC;
      };
      break;
    case PC_07:
      {
        GPIO_Pin = GPIO_Pin_7;
        port = GPIOC;
      };
      break;
    case PD_00:
      {
        GPIO_Pin = GPIO_Pin_0;
        port = GPIOD;
      };
      break;
    case PD_01:
      {
        GPIO_Pin = GPIO_Pin_1;
        port = GPIOD;
      };
      break;
    case PD_02:
      {
        GPIO_Pin = GPIO_Pin_2;
        port = GPIOD;
      };
      break;
    case PD_03:
      {
        GPIO_Pin = GPIO_Pin_3;
        port = GPIOD;
      };
      break;
    case PD_04:
      {
        GPIO_Pin = GPIO_Pin_4;
        port = GPIOD;
      };
      break;
    case PD_05:
      {
        GPIO_Pin = GPIO_Pin_5;
        port = GPIOD;
      };
      break;
    case PD_06:
      {
        GPIO_Pin = GPIO_Pin_6;
        port = GPIOD;
      };
      break;
    case PD_07:
      {
        GPIO_Pin = GPIO_Pin_7;
        port = GPIOD;
      };
      break;
    }

  port_i2c_sda = port;
  pin_i2c_sda = GPIO_Pin;

  GPIO_InitTypeDef GPIO_InitStructure = { 0 };
  if (port_i2c_sda == GPIOA)
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
  if (port_i2c_sda == GPIOC)
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC, ENABLE);
  if (port_i2c_sda == GPIOD)
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin = pin_i2c_sda;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // open-drain mode as needed
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
  GPIO_Init (port_i2c_sda, &GPIO_InitStructure);

  if (port_i2c_scl == GPIOA)
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
  if (port_i2c_scl == GPIOC)
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC, ENABLE);
  if (port_i2c_scl == GPIOD)
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin = pin_i2c_scl;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // open-drain mode as needed
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
  GPIO_Init (port_i2c_scl, &GPIO_InitStructure);

  IIC_SDA (1);
  IIC_SCL (1);
}

static void
SDA_IN (void)
{
  GPIO_InitTypeDef GPIO_InitStructure = { 0 };
  GPIO_InitStructure.GPIO_Pin = pin_i2c_sda;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
  GPIO_Init (port_i2c_sda, &GPIO_InitStructure);
}

static void
SDA_OUT (void)
{
  GPIO_InitTypeDef GPIO_InitStructure = { 0 };
  GPIO_InitStructure.GPIO_Pin = pin_i2c_sda;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // open-drain mode as needed
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
  GPIO_Init (port_i2c_sda, &GPIO_InitStructure);
}

static void
IIC_Start (void)
{
  SDA_OUT ();
  IIC_SDA (1);
  IIC_SCL (1);
  delay_us (4);
  IIC_SDA (0);
  delay_us (4);
  IIC_SCL (0);
}

static void
IIC_Stop (void)
{
  SDA_OUT ();
  IIC_SCL (0);
  IIC_SDA (0);
  delay_us (4);
  IIC_SCL (1);
  delay_us (4);
  IIC_SDA (1);
}

static unsigned char
IIC_Wait_Ack (void)
{
  unsigned char ucErrTime = 0;
  SDA_IN ();
  IIC_SDA (1);
  delay_us (1);
  IIC_SCL (1);
  delay_us (1);
  while (READ_SDA)
    {
      ucErrTime++;
      if (ucErrTime > 250)
        {
          IIC_Stop ();
          return 1;
        }
    }
  IIC_SCL (0);
  return 0;
}

static void
IIC_Ack (void)
{
  IIC_SCL (0);
  SDA_OUT ();
  IIC_SDA (0);
  delay_us (2);
  IIC_SCL (1);
  delay_us (2);
  IIC_SCL (0);
}

static void
IIC_NAck (void)
{
  IIC_SCL (0);
  SDA_OUT ();
  IIC_SDA (1);
  delay_us (2);
  IIC_SCL (1);
  delay_us (2);
  IIC_SCL (0);
}

static void
IIC_Send_Byte (unsigned char txd)
{
  unsigned char t;
  SDA_OUT ();
  IIC_SCL (0);
  for (t = 0; t < 8; t++)
    {
      IIC_SDA ((txd & 0x80) >> 7);
      txd <<= 1;
      delay_us (2);
      IIC_SCL (1);
      delay_us (2);
      IIC_SCL (0);
      delay_us (2);
    }
}

static unsigned char
IIC_Read_Byte (unsigned char ack)
{
  unsigned char i, receive = 0;
  SDA_IN ();
  for (i = 0; i < 8; i++)
    {
      IIC_SCL (0);
      delay_us (2);
      IIC_SCL (1);
      receive <<= 1;
      if (READ_SDA)
        receive++;
      delay_us (1);
    }
  if (!ack)
    IIC_NAck ();
  else
    IIC_Ack ();
  return receive;
}

static int
SWire_write_regs (i2c_t dev, uint16_t addr, uint16_t reg, const void *data,
                  size_t len, uint8_t flags)
{
  IIC_Start ();

  IIC_Send_Byte ((addr << 1) | 0);
  if (IIC_Wait_Ack ())
    {
      IIC_Stop ();
      return 1;
    }

  if (flags & I2C_REG16)
    {
      reg = htons (reg); /* Make sure register is in big-endian on I2C bus */
      IIC_Send_Byte (reg & 0xFF);
      IIC_Wait_Ack ();
      IIC_Send_Byte (((reg & 0xFF00) >> 8));
      IIC_Wait_Ack ();
    }
  else
    {
      IIC_Send_Byte (reg);
      IIC_Wait_Ack ();
    }

  for (u16 u8_index = 0; u8_index < len; u8_index++)
    {
      u8 dat = ((u8 *)data)[u8_index];
      IIC_Send_Byte (dat);
      if (IIC_Wait_Ack ())
        {
          IIC_Stop ();
          return 1;
        }
    }
  IIC_Stop ();
  return 0;
}

static int
SWire_read_regs (i2c_t dev, uint16_t addr, uint16_t reg, void *data,
                 size_t len, uint8_t flags)
{
  IIC_Start ();
  IIC_Send_Byte ((addr << 1) | 0);
  IIC_Wait_Ack ();

  if (flags & I2C_REG16)
    {
      reg = htons (reg); /* Make sure register is in big-endian on I2C bus */
      IIC_Send_Byte (reg & 0xFF);
      IIC_Wait_Ack ();
      IIC_Send_Byte (((reg & 0xFF00) >> 8));
      IIC_Wait_Ack ();
    }
  else
    {
      IIC_Send_Byte (reg);
      IIC_Wait_Ack ();
    }

  IIC_Start ();
  IIC_Send_Byte ((addr << 1) | 1);
  IIC_Wait_Ack ();

  u8 *buf = (u8 *)data;
  while (len > 1)
    {
      *buf++ = IIC_Read_Byte (1);
      // printf("\nread byte=%x\n",*(pBuffer - 1));
      len--;
    }
  *buf = IIC_Read_Byte (0);
  IIC_Stop ();
  return 0;
}

static int
SWire_write_bytes (i2c_t dev, uint16_t addr, const void *data, size_t len,
                   uint8_t flags)
{
  IIC_Start ();

  IIC_Send_Byte ((addr << 1) | 0);
  if (IIC_Wait_Ack ())
    {
      IIC_Stop ();
      return 1;
    }

  for (u16 u8_index = 0; u8_index < len; u8_index++)
    {
      u8 dat = ((u8 *)data)[u8_index];
      IIC_Send_Byte (dat);
      if (IIC_Wait_Ack ())
        {
          IIC_Stop ();
          return 1;
        }
    }
  IIC_Stop ();
  return 0;
}

static int
SWire_read_bytes (i2c_t dev, uint16_t addr, void *data, size_t len,
                  uint8_t flags)
{
  IIC_Start ();
  IIC_Send_Byte ((addr << 1) | 0);
  IIC_Wait_Ack ();

  IIC_Start ();
  IIC_Send_Byte ((addr << 1) | 1);
  IIC_Wait_Ack ();

  u8 *buf = (u8 *)data;
  while (len > 1)
    {
      *buf++ = IIC_Read_Byte (1);
      // printf("\nread byte=%x\n",*(pBuffer - 1));
      len--;
    }
  *buf = IIC_Read_Byte (0);
  IIC_Stop ();
  return 0;
}

//************************************************************************8
