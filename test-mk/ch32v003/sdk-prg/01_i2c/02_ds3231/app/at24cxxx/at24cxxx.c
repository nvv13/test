/*

Взято из
https://github.com/RIOT-OS/RIOT

*/

/*
 * Copyright (C) 2019 Otto-von-Guericke-Universität Magdeburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_at24cxxx
 * @{
 *
 * @file
 * @brief       Device driver implementation for at24cxxx EEPROM units.
 *
 * @author      Fabian Hüßler <fabian.huessler@ovgu.de>
 * @}
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>


#include "assert.h"

#include "at24cxxx.h"
//#include "at24cxxx_defines.h"

#include "debug.h"


#define ENABLE_DEBUG_AT 0

#if (ENABLE_DEBUG_AT==1)
#define DEBUG_AT(format, ...) printf (format, ##__VA_ARGS__)
#else
#define DEBUG_AT(format, ...) Delay_Us(5000)
#endif



#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

/**
 * @brief Calculate x mod y, if y is a power of 2
 */
#define MOD_POW2(x, y) ((x) & ((y)-1))

/**
 * @brief I2C bus number shortcut
 */
#define DEV_I2C_BUS (dev->params.i2c)
/**
 * @brief Pin wp shortcut
 */
#define DEV_PIN_WP (dev->params.pin_wp)
/**
 * @brief EEPROM size shortcut
 */
#define DEV_EEPROM_SIZE (dev->params.eeprom_size)
/**
 * @brief I2C device address shortcut
 */
#define DEV_I2C_ADDR (dev->params.dev_addr)
/**
 * @brief Page size shortcut
 */
#define DEV_PAGE_SIZE (dev->params.page_size)
/**
 * @brief Max polls shortcut
 */
#define DEV_MAX_POLLS (dev->params.max_polls)

#ifndef AT24CXXX_SET_BUF_SIZE
/**
 * @brief  Adjust to configure buffer size
 */
#define AT24CXXX_SET_BUF_SIZE (32U)

#endif

//void Delay_Us(
//static uint8_t p_us = 0;

//static void
//xtimer_usleep (u32 us)
//{
//  if (us == 0)
//    return;
//  uint32_t i = SysTick->CNT + (uint32_t)us * p_us;
//  SysTick->CTLR |=(1 << 0);
//  while (SysTick->CNT < i)
//    ;
//}

#define GPIO_OUT GPIO_Mode_Out_PP

static void
gpio_init (pin_t pin, u8 dir)
{
    if(pin==GPIO_UNDEF)return;
    GPIO_TypeDef *port=GPIOA;
    u8 GPIO_Pin=0;
    switch(pin)
    {
    case PA_00: {GPIO_Pin=GPIO_Pin_0;port=GPIOA;};break;
    case PA_01: {GPIO_Pin=GPIO_Pin_1;port=GPIOA;};break;
    case PA_02: {GPIO_Pin=GPIO_Pin_2;port=GPIOA;};break;
    case PA_03: {GPIO_Pin=GPIO_Pin_3;port=GPIOA;};break;
    case PA_04: {GPIO_Pin=GPIO_Pin_4;port=GPIOA;};break;
    case PA_05: {GPIO_Pin=GPIO_Pin_5;port=GPIOA;};break;
    case PA_06: {GPIO_Pin=GPIO_Pin_6;port=GPIOA;};break;
    case PA_07: {GPIO_Pin=GPIO_Pin_7;port=GPIOA;};break;
    case PC_00: {GPIO_Pin=GPIO_Pin_0;port=GPIOC;};break;
    case PC_01: {GPIO_Pin=GPIO_Pin_1;port=GPIOC;};break;
    case PC_02: {GPIO_Pin=GPIO_Pin_2;port=GPIOC;};break;
    case PC_03: {GPIO_Pin=GPIO_Pin_3;port=GPIOC;};break;
    case PC_04: {GPIO_Pin=GPIO_Pin_4;port=GPIOC;};break;
    case PC_05: {GPIO_Pin=GPIO_Pin_5;port=GPIOC;};break;
    case PC_06: {GPIO_Pin=GPIO_Pin_6;port=GPIOC;};break;
    case PC_07: {GPIO_Pin=GPIO_Pin_7;port=GPIOC;};break;
    case PD_00: {GPIO_Pin=GPIO_Pin_0;port=GPIOD;};break;
    case PD_01: {GPIO_Pin=GPIO_Pin_1;port=GPIOD;};break;
    case PD_02: {GPIO_Pin=GPIO_Pin_2;port=GPIOD;};break;
    case PD_03: {GPIO_Pin=GPIO_Pin_3;port=GPIOD;};break;
    case PD_04: {GPIO_Pin=GPIO_Pin_4;port=GPIOD;};break;
    case PD_05: {GPIO_Pin=GPIO_Pin_5;port=GPIOD;};break;
    case PD_06: {GPIO_Pin=GPIO_Pin_6;port=GPIOD;};break;
    case PD_07: {GPIO_Pin=GPIO_Pin_7;port=GPIOD;};break;
    }
  GPIO_InitTypeDef GPIO_InitStructure = { 0 };
  GPIOSpeed_TypeDef GPIO_Speed = GPIO_Speed_50MHz;
    if (port == GPIOA)
      RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
    if (port == GPIOC)
      RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC, ENABLE);
    if (port == GPIOD)
      RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
    GPIO_Init (port, &GPIO_InitStructure);
};

static void
gpio_set (pin_t pin)
{
    if(pin==GPIO_UNDEF)return;
    GPIO_TypeDef *port=GPIOA;
    u8 GPIO_Pin=0;
    switch(pin)
    {
    case PA_00: {GPIO_Pin=GPIO_Pin_0;port=GPIOA;};break;
    case PA_01: {GPIO_Pin=GPIO_Pin_1;port=GPIOA;};break;
    case PA_02: {GPIO_Pin=GPIO_Pin_2;port=GPIOA;};break;
    case PA_03: {GPIO_Pin=GPIO_Pin_3;port=GPIOA;};break;
    case PA_04: {GPIO_Pin=GPIO_Pin_4;port=GPIOA;};break;
    case PA_05: {GPIO_Pin=GPIO_Pin_5;port=GPIOA;};break;
    case PA_06: {GPIO_Pin=GPIO_Pin_6;port=GPIOA;};break;
    case PA_07: {GPIO_Pin=GPIO_Pin_7;port=GPIOA;};break;
    case PC_00: {GPIO_Pin=GPIO_Pin_0;port=GPIOC;};break;
    case PC_01: {GPIO_Pin=GPIO_Pin_1;port=GPIOC;};break;
    case PC_02: {GPIO_Pin=GPIO_Pin_2;port=GPIOC;};break;
    case PC_03: {GPIO_Pin=GPIO_Pin_3;port=GPIOC;};break;
    case PC_04: {GPIO_Pin=GPIO_Pin_4;port=GPIOC;};break;
    case PC_05: {GPIO_Pin=GPIO_Pin_5;port=GPIOC;};break;
    case PC_06: {GPIO_Pin=GPIO_Pin_6;port=GPIOC;};break;
    case PC_07: {GPIO_Pin=GPIO_Pin_7;port=GPIOC;};break;
    case PD_00: {GPIO_Pin=GPIO_Pin_0;port=GPIOD;};break;
    case PD_01: {GPIO_Pin=GPIO_Pin_1;port=GPIOD;};break;
    case PD_02: {GPIO_Pin=GPIO_Pin_2;port=GPIOD;};break;
    case PD_03: {GPIO_Pin=GPIO_Pin_3;port=GPIOD;};break;
    case PD_04: {GPIO_Pin=GPIO_Pin_4;port=GPIOD;};break;
    case PD_05: {GPIO_Pin=GPIO_Pin_5;port=GPIOD;};break;
    case PD_06: {GPIO_Pin=GPIO_Pin_6;port=GPIOD;};break;
    case PD_07: {GPIO_Pin=GPIO_Pin_7;port=GPIOD;};break;
    }
    GPIO_WriteBit (port, GPIO_Pin, Bit_SET);
};
static void
gpio_clear (pin_t pin)
{
    if(pin==GPIO_UNDEF)return;
    GPIO_TypeDef *port=GPIOA;
    u8 GPIO_Pin=0;
    switch(pin)
    {
    case PA_00: {GPIO_Pin=GPIO_Pin_0;port=GPIOA;};break;
    case PA_01: {GPIO_Pin=GPIO_Pin_1;port=GPIOA;};break;
    case PA_02: {GPIO_Pin=GPIO_Pin_2;port=GPIOA;};break;
    case PA_03: {GPIO_Pin=GPIO_Pin_3;port=GPIOA;};break;
    case PA_04: {GPIO_Pin=GPIO_Pin_4;port=GPIOA;};break;
    case PA_05: {GPIO_Pin=GPIO_Pin_5;port=GPIOA;};break;
    case PA_06: {GPIO_Pin=GPIO_Pin_6;port=GPIOA;};break;
    case PA_07: {GPIO_Pin=GPIO_Pin_7;port=GPIOA;};break;
    case PC_00: {GPIO_Pin=GPIO_Pin_0;port=GPIOC;};break;
    case PC_01: {GPIO_Pin=GPIO_Pin_1;port=GPIOC;};break;
    case PC_02: {GPIO_Pin=GPIO_Pin_2;port=GPIOC;};break;
    case PC_03: {GPIO_Pin=GPIO_Pin_3;port=GPIOC;};break;
    case PC_04: {GPIO_Pin=GPIO_Pin_4;port=GPIOC;};break;
    case PC_05: {GPIO_Pin=GPIO_Pin_5;port=GPIOC;};break;
    case PC_06: {GPIO_Pin=GPIO_Pin_6;port=GPIOC;};break;
    case PC_07: {GPIO_Pin=GPIO_Pin_7;port=GPIOC;};break;
    case PD_00: {GPIO_Pin=GPIO_Pin_0;port=GPIOD;};break;
    case PD_01: {GPIO_Pin=GPIO_Pin_1;port=GPIOD;};break;
    case PD_02: {GPIO_Pin=GPIO_Pin_2;port=GPIOD;};break;
    case PD_03: {GPIO_Pin=GPIO_Pin_3;port=GPIOD;};break;
    case PD_04: {GPIO_Pin=GPIO_Pin_4;port=GPIOD;};break;
    case PD_05: {GPIO_Pin=GPIO_Pin_5;port=GPIOD;};break;
    case PD_06: {GPIO_Pin=GPIO_Pin_6;port=GPIOD;};break;
    case PD_07: {GPIO_Pin=GPIO_Pin_7;port=GPIOD;};break;
    }
    GPIO_WriteBit (port, GPIO_Pin, Bit_RESET);
};


static int
_read (const at24cxxx_t *dev, uint32_t pos, void *data, size_t len)
{
  int check;
  uint8_t polls = DEV_MAX_POLLS;
  uint8_t dev_addr;
  uint8_t flags = 0;

  if (DEV_EEPROM_SIZE > 2048)
    {
      /* 2 bytes word address length if more than 11 bits are
         used for addressing */
      /* append page address bits to device address (if any) */
      dev_addr = (DEV_I2C_ADDR | ((pos & 0xFF0000) >> 16));
      pos &= 0xFFFF;
      flags = I2C_REG16;
    }
  else
    {
      /* append page address bits to device address (if any) */
      dev_addr = (DEV_I2C_ADDR | ((pos & 0xFF00) >> 8));
      pos &= 0xFF;
    }

  while (-ENXIO
         == (check
             = i2c_read_regs (DEV_I2C_BUS, dev_addr, pos, data, len, flags)))
    {
      if (--polls == 0)
        {
          break;
        }
      Delay_Us (AT24CXXX_POLL_DELAY_US);
    }

  DEBUG_AT ("[at24cxxx] i2c_read_regs(): %d; polls: %d\n", check, polls);

  return check;
}

static int
_read_max (const at24cxxx_t *dev, uint32_t pos, void *data, size_t len)
{
#ifdef PERIPH_I2C_MAX_BYTES_PER_FRAME
  uint8_t *data_p = data;

  while (len)
    {
      size_t clen = MIN (len, PERIPH_I2C_MAX_BYTES_PER_FRAME);

      if (_read (dev, pos, data_p, clen) == AT24CXXX_OK)
        {
          len -= clen;
          pos += clen;
          data_p += clen;
        }
      else
        {
          return -EIO;
        }
    }

  return AT24CXXX_OK;
#else
  return _read (dev, pos, data, len);
#endif
}

static int
_write_page (const at24cxxx_t *dev, uint32_t pos, const void *data, size_t len)
{
  int check;
  uint8_t polls = DEV_MAX_POLLS;
  uint8_t dev_addr;
  uint8_t flags = 0;

  if (DEV_EEPROM_SIZE > 2048)
    {
      /* 2 bytes word address length if more than 11 bits are
         used for addressing */
      /* append page address bits to device address (if any) */
      dev_addr = (DEV_I2C_ADDR | ((pos & 0xFF0000) >> 16));
      pos &= 0xFFFF;
      flags = I2C_REG16;
    }
  else
    {
      /* append page address bits to device address (if any) */
      dev_addr = (DEV_I2C_ADDR | ((pos & 0xFF00) >> 8));
      pos &= 0xFF;
    }

  while (-ENXIO
         == (check
             = i2c_write_regs (DEV_I2C_BUS, dev_addr, pos, data, len, flags)))
    {
      if (--polls == 0)
        {
          break;
        }
      Delay_Us (AT24CXXX_POLL_DELAY_US);
    }

  DEBUG_AT ("[at24cxxx] i2c_write_regs(): %d; polls: %d\n", check, polls);

  return check;
}

static int
_write (const at24cxxx_t *dev, uint32_t pos, const void *data, size_t len)
{
  int check = 0;
  const uint8_t *cdata = ((const uint8_t *)data);

  while (len)
    {
      size_t clen = MIN (len, DEV_PAGE_SIZE - MOD_POW2 (pos, DEV_PAGE_SIZE));

      check = _write_page (dev, pos, cdata, clen);

      if (!check)
        {
          len -= clen;
          pos += clen;
          cdata += clen;
        }
      else
        {
          break;
        }
    }
  return check;
}

static int
_set (const at24cxxx_t *dev, uint32_t pos, uint8_t val, size_t len)
{
  int check;
  uint8_t set_buffer[AT24CXXX_SET_BUF_SIZE];

  memset (set_buffer, val, sizeof (set_buffer));
  while (len)
    {
      size_t clen = MIN (sizeof (set_buffer), len);
      check = _write (dev, pos, set_buffer, clen);
      if (!check)
        {
          len -= clen;
          pos += clen;
        }
      else
        {
          break;
        }
    }
  return check;
}

int
at24cxxx_init (at24cxxx_t *dev, const at24cxxx_params_t *params)
{
  if (!dev || !params)
    {
      return -EINVAL;
    }
  dev->params = *params;
  //p_us = SystemCoreClock / 8000000;
  Delay_Init ();

  if (gpio_is_valid (DEV_PIN_WP))
    {
      gpio_init (DEV_PIN_WP, GPIO_OUT);
      at24cxxx_disable_write_protect (dev);
    }
  /* Check I2C bus once */
  i2c_acquire (DEV_I2C_BUS);
  i2c_release (DEV_I2C_BUS);
  return AT24CXXX_OK;
}

int
at24cxxx_read_byte (const at24cxxx_t *dev, uint32_t pos, void *dest)
{
  if (pos >= DEV_EEPROM_SIZE)
    {
      return -ERANGE;
    }

  i2c_acquire (DEV_I2C_BUS);
  int check = _read (dev, pos, dest, 1);
  i2c_release (DEV_I2C_BUS);
  return check;
}

int
at24cxxx_read (const at24cxxx_t *dev, uint32_t pos, void *data, size_t len)
{
  if (pos + len > DEV_EEPROM_SIZE)
    {
      return -ERANGE;
    }

  int check = AT24CXXX_OK;
  if (len)
    {
      i2c_acquire (DEV_I2C_BUS);
      check = _read_max (dev, pos, data, len);
      i2c_release (DEV_I2C_BUS);
    }

  return check;
}

int
at24cxxx_write_byte (const at24cxxx_t *dev, uint32_t pos, uint8_t data)
{
  if (pos >= DEV_EEPROM_SIZE)
    {
      return -ERANGE;
    }

  i2c_acquire (DEV_I2C_BUS);
  int check = _write (dev, pos, &data, 1);
  i2c_release (DEV_I2C_BUS);
  return check;
}

int
at24cxxx_write (const at24cxxx_t *dev, uint32_t pos, const void *data,
                size_t len)
{
  if (pos + len > DEV_EEPROM_SIZE)
    {
      return -ERANGE;
    }

  int check = AT24CXXX_OK;
  if (len)
    {
      i2c_acquire (DEV_I2C_BUS);
      check = _write (dev, pos, data, len);
      i2c_release (DEV_I2C_BUS);
    }
  return check;
}

int
at24cxxx_set (const at24cxxx_t *dev, uint32_t pos, uint8_t val, size_t len)
{
  if (pos + len > DEV_EEPROM_SIZE)
    {
      return -ERANGE;
    }

  int check = AT24CXXX_OK;
  if (len)
    {
      i2c_acquire (DEV_I2C_BUS);
      check = _set (dev, pos, val, len);
      i2c_release (DEV_I2C_BUS);
    }
  return check;
}

int
at24cxxx_clear (const at24cxxx_t *dev, uint32_t pos, size_t len)
{
  return at24cxxx_set (dev, pos, AT24CXXX_CLEAR_BYTE, len);
}

int
at24cxxx_erase (const at24cxxx_t *dev)
{
  return at24cxxx_clear (dev, 0, DEV_EEPROM_SIZE);
}

int
at24cxxx_enable_write_protect (const at24cxxx_t *dev)
{
  if (!gpio_is_valid (DEV_PIN_WP))
    {
      return -ENOTSUP;
    }
  gpio_set (DEV_PIN_WP);
  return AT24CXXX_OK;
}

int
at24cxxx_disable_write_protect (const at24cxxx_t *dev)
{
  if (!gpio_is_valid (DEV_PIN_WP))
    {
      return -ENOTSUP;
    }
  gpio_clear (DEV_PIN_WP);
  return AT24CXXX_OK;
}
