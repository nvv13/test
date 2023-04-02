/*
 * Copyright (C) 2016-2018 Bas Stottelaar <basstottelaar@gmail.com>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     pkg_u8g2
 * @{
 *
 * @file
 * @brief       U8g2 driver for interacting with RIOT-OS peripherals
 *
 * @author      Bas Stottelaar <basstottelaar@gmail.com>
 *
 * @}
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "u8x8_riotos.h"

#include "n_delay.h"
#include "wm_gpio.h"
#include "wm_gpio_afsel.h"

#include "wm_hostspi.h"
#include "wm_i2c.h"

static uint32_t
u8x8_pulse_width_to_spi_speed (uint32_t pulse_width)
{
  const uint32_t cycle_time = 2 * pulse_width;

  if (cycle_time < 100)
    {
      return 10000000; // SPI_CLK_10MHZ;
    }
  else if (cycle_time < 200)
    {
      return 5000000; // SPI_CLK_5MHZ;
    }
  else if (cycle_time < 1000)
    {
      return 1000000; // SPI_CLK_1MHZ;
    }
  else if (cycle_time < 2500)
    {
      return 400000; // SPI_CLK_400KHZ;
    }

  return 100000; // SPI_CLK_100KHZ;
}

static u8
u8x8_spi_mode_to_spi_conf (uint32_t spi_mode)
{
  return spi_mode;
}

/**
 * @brief   Enable the selected pins in RIOT-OS.
 */
static void
_enable_pins (const u8x8_riotos_t *u8x8_riot_ptr)
{
  /* no hardware peripheral is being used, nothing to be done */
  if (u8x8_riot_ptr == NULL)
    {
      return;
    }
  if (gpio_is_valid (u8x8_riot_ptr->pin_cs))
    {
      tls_gpio_cfg (u8x8_riot_ptr->pin_cs, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_PULLHIGH);
    }

  if (gpio_is_valid (u8x8_riot_ptr->pin_dc))
    {
      tls_gpio_cfg (u8x8_riot_ptr->pin_dc, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_PULLHIGH);
    }

  if (gpio_is_valid (u8x8_riot_ptr->pin_reset))
    {
      tls_gpio_cfg (u8x8_riot_ptr->pin_reset, WM_GPIO_DIR_OUTPUT,
                    WM_GPIO_ATTR_PULLHIGH);
    }
}

uint8_t
u8x8_gpio_and_delay_riotos (u8x8_t *u8g2, uint8_t msg, uint8_t arg_int,
                            void *arg_ptr)
{
  (void)arg_ptr;

  const u8x8_riotos_t *u8x8_riot_ptr = u8x8_GetUserPtr (u8g2);

  /* assert that user_ptr is correctly set */
  assert (u8x8_riot_ptr != NULL);

  switch (msg)
    {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      _enable_pins (u8x8_riot_ptr);
      break;
    case U8X8_MSG_DELAY_MILLI:
      //            xtimer_usleep(arg_int * 1000);
      n_delay_ms (arg_int);
      break;
    case U8X8_MSG_DELAY_10MICRO:
      //            xtimer_usleep(arg_int * 10);
      n_delay_us (arg_int * 10);
      break;
    case U8X8_MSG_DELAY_100NANO:
      //            xtimer_nanosleep(arg_int * 100);
      n_delay_us (arg_int);
      break;
    case U8X8_MSG_GPIO_CS:
      if (u8x8_riot_ptr != NULL && gpio_is_valid (u8x8_riot_ptr->pin_cs))
        {
          tls_gpio_write (u8x8_riot_ptr->pin_cs,
                          arg_int); //(arg_int==0?1:0) );
        }
      break;
    case U8X8_MSG_GPIO_DC:
      if (u8x8_riot_ptr != NULL && gpio_is_valid (u8x8_riot_ptr->pin_dc))
        {
          tls_gpio_write (u8x8_riot_ptr->pin_dc, arg_int);
        }
      break;
    case U8X8_MSG_GPIO_RESET:
      if (u8x8_riot_ptr != NULL && gpio_is_valid (u8x8_riot_ptr->pin_reset))
        {
          tls_gpio_write (u8x8_riot_ptr->pin_reset, arg_int);
        }
      break;
    default:
      return 0;
    }

  return 1;
}

uint8_t
u8x8_byte_hw_spi_riotos (u8x8_t *u8g2, uint8_t msg, uint8_t arg_int,
                         void *arg_ptr)
{
  const u8x8_riotos_t *u8x8_riot_ptr = u8x8_GetUserPtr (u8g2);

  /* assert that user_ptr is correctly set */
  assert (u8x8_riot_ptr != NULL);

  switch (msg)
    {
    case U8X8_MSG_BYTE_SEND:
      // spi_transfer_bytes(dev, GPIO_UNDEF, true,
      //                   arg_ptr, NULL, (size_t)arg_int);
      tls_spi_write ((u8 *)arg_ptr, (size_t)arg_int);
      break;
    case U8X8_MSG_BYTE_INIT:
      break;
    case U8X8_MSG_BYTE_SET_DC:
      u8x8_gpio_SetDC (u8g2, arg_int);
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:

      // spi_acquire(dev, GPIO_UNDEF,
      //            u8x8_spi_mode_to_spi_conf(u8g2->display_info->spi_mode),
      //            u8x8_pulse_width_to_spi_speed(u8g2->display_info->sck_pulse_width_ns));
      /*MASTER SPI configuratioin*/

      wm_spi_cs_config (u8x8_riot_ptr->spi_cs);
      wm_spi_ck_config (u8x8_riot_ptr->spi_ck);
      wm_spi_di_config (u8x8_riot_ptr->spi_di);
      wm_spi_do_config (u8x8_riot_ptr->spi_do);
      // printf (
      //"MASTER SPI configuratioin cs--PB14, ck--PB15, di--PB16,
      // do--PB17;\r\n");
      tls_spi_trans_type (SPI_DMA_TRANSFER); // byte , word, dma
      tls_spi_setup (u8x8_spi_mode_to_spi_conf (
                         u8g2->display_info->spi_mode), // TLS_SPI_MODE_3,
                     TLS_SPI_CS_LOW,                   // TLS_SPI_CS_HIGH,
                     u8x8_pulse_width_to_spi_speed (
                         u8g2->display_info->sck_pulse_width_ns) // clk 400000
      );

      u8x8_gpio_SetCS (u8g2, u8g2->display_info->chip_enable_level);
      u8g2->gpio_and_delay_cb (u8g2, U8X8_MSG_DELAY_NANO,
                               u8g2->display_info->post_chip_enable_wait_ns,
                               NULL);

      break;
    case U8X8_MSG_BYTE_END_TRANSFER:

      u8g2->gpio_and_delay_cb (u8g2, U8X8_MSG_DELAY_NANO,
                               u8g2->display_info->pre_chip_disable_wait_ns,
                               NULL);
      u8x8_gpio_SetCS (u8g2, u8g2->display_info->chip_disable_level);

      // spi_release(dev);

      break;
    default:
      return 0;
    }

  return 1;
}

static u8
wire_beginTransmission (u8 AddresI2C)
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
wire_write (u8 *cmdI2C, u8 u8_Len)
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
wire_endTransmission (void)
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

uint8_t
u8x8_byte_hw_i2c_riotos (u8x8_t *u8g2, uint8_t msg, uint8_t arg_int,
                         void *arg_ptr)
{
  static uint8_t buffer[32]; /* u8x8 will never send more than 32 bytes
                                between START_TRANSFER and END_TRANSFER */
  static size_t index;

  const u8x8_riotos_t *u8x8_riot_ptr = u8x8_GetUserPtr (u8g2);

  /* assert that user_ptr is correctly set */
  assert (u8x8_riot_ptr != NULL);

  switch (msg)
    {
    case U8X8_MSG_BYTE_SEND:
      memcpy (&buffer[index], arg_ptr, arg_int);
      index += arg_int;
      assert (index <= sizeof (buffer));
      break;
    case U8X8_MSG_BYTE_INIT:
      break;
    case U8X8_MSG_BYTE_SET_DC:
      break;
    case U8X8_MSG_BYTE_START_TRANSFER:
      //              i2c_acquire(dev);
      wm_i2c_scl_config (u8x8_riot_ptr->i2c_scl);
      wm_i2c_sda_config (u8x8_riot_ptr->i2c_sda);
      tls_i2c_init (u8x8_riot_ptr->i2c_freq);
      index = 0;
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      //      i2c_write_bytes (dev, u8x8_GetI2CAddress (u8g2), buffer, index,
      //      0);
      wire_beginTransmission (u8x8_GetI2CAddress (u8g2));
      wire_write (buffer, index);
      wire_endTransmission ();

      //      i2c_release (dev);

      break;
    default:
      return 0;
    }

  return 1;
}
