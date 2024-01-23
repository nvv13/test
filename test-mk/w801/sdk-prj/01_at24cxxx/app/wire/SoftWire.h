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

#include "csi_core.h"

#include "wm_cpu.h"

extern uint32_t csi_coret_get_load (void);
extern uint32_t csi_coret_get_value (void);

static void _n_delay_us (uint32_t us);

/**
 * @brief     interface delay ms (Milliseconds 10^-3)
 * @param[in] ms
 * @note      none
 */
static void
_n_delay_ms (uint32_t ms)
{
  if (ms == 0)
    return;
  do
    {
      _n_delay_us (1000);
    }
  while ((--ms) > 0);
}

/**
 * @brief     interface delay us (Microseconds 10^-6)
 * @param[in] us
 * @note      none
 */
static void
_n_delay_us (uint32_t us)
{
  if (us > 1000) //надо так, иначе зависнет, уже 2000 перебор, поэтому так
    {
      _n_delay_ms (us / 1000);
      us = us % 1000;
    }
  if (us == 0)
    return;

  uint32_t load = csi_coret_get_load ();
  uint32_t start = csi_coret_get_value ();
  uint32_t cur;
  uint32_t cnt;
  tls_sys_clk sysclk;

  tls_sys_clk_get (&sysclk);
  cnt = sysclk.cpuclk * us;
  /*
  при 240MHz значение sysclk.cpuclk=240
  то есть
  240 cpuclk = 1 микросекунда
  или 1MHz,
  то есть
  System Tick Timer (CORET) - меняеться с частотой CPU в сторону уменьшения
  csi_coret_get_value() - текущее значение CORET

  при достижении нуля, возникает прерываение, и загрузка текущего значения из
  переменой csi_coret_get_load() - настраевает OS при смене частоты процессора

  значение load от частоты:
  240MHz load=479999
  160MHz load=319999
  80MHz  load=159999
  40MHz  load=79999

  если частоту в герцах поделить на значение load,
  то это и будет 500 герц (т.е. период 2 милисекунды)

как считает тики, например:
--------------------------------------------------
 если	    | load |start|  cur |формула       |=|
--------------------------------------------------
start > cur |479999|7000 |  6998|start-cur     |2|
--------------------------------------------------
start <= cur|479999|1    |479998|load-cur+start|2|
--------------------------------------------------
  */

  while (1)
    {
      cur = csi_coret_get_value ();

      if (start > cur)
        {
          if (start - cur >= cnt)
            {
              return;
            }
        }
      else
        {
          if (load - cur + start > cnt)
            {
              return;
            }
        }
    }
}

static enum tls_io_name _i2c_scl;
static enum tls_io_name _i2c_sda;

#define IIC_SCL(n)                                                            \
  (n ? tls_gpio_write (_i2c_scl, 1) : tls_gpio_write (_i2c_scl, 0))
#define IIC_SDA(n)                                                            \
  (n ? tls_gpio_write (_i2c_sda, 1) : tls_gpio_write (_i2c_sda, 0))
#define READ_SDA tls_gpio_read (_i2c_sda)

#define delay_us(n) _n_delay_us (n)

static void
SWire_init (i2c_t dev)
{
  _i2c_scl = dev->i2c_scl;
  _i2c_sda = dev->i2c_sda;
  tls_gpio_cfg (dev->i2c_scl, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
  tls_gpio_cfg (dev->i2c_sda, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);

  IIC_SDA (1);
  IIC_SCL (1);
}

static void
SDA_IN (void)
{
  tls_gpio_cfg (_i2c_sda, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_PULLHIGH);
}

static void
SDA_OUT (void)
{
  tls_gpio_cfg (_i2c_sda, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLHIGH);
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



static int SWire_write_regs(i2c_t dev, uint16_t addr, uint16_t reg,
                  const void *data, size_t len, uint8_t flags)
{
  _i2c_scl = dev->i2c_scl;
  _i2c_sda = dev->i2c_sda;
  IIC_Start ();

  IIC_Send_Byte ((addr << 1) | 0);
  if (IIC_Wait_Ack ())
    {
      IIC_Stop ();
      return 1;
    }

  if (flags & I2C_REG16) 
    {
      reg = htons(reg); /* Make sure register is in big-endian on I2C bus */
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
      u8 dat = ((u8*)data)[u8_index];
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

static int SWire_read_regs(i2c_t dev, uint16_t addr, uint16_t reg,
                  void *data, size_t len, uint8_t flags)
{
  _i2c_scl = dev->i2c_scl;
  _i2c_sda = dev->i2c_sda;
  IIC_Start ();
  IIC_Send_Byte ((addr << 1) | 0);
  IIC_Wait_Ack ();

  if (flags & I2C_REG16) 
    {
      reg = htons(reg); /* Make sure register is in big-endian on I2C bus */
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

  u8* buf=(u8*)data;
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
