/**
 * @ingroup     drivers_ws2812b
 * @{
 *
 * @file
 * @brief       WS2812B RGB LED driver implementation
 *
 *
 * @}
 */

#include <string.h>

#include "assert.h"
#include "wm_type_def.h"

#include "wm_cpu.h"
#include "wm_gpio.h"
#include "wm_i2s.h"
#include "wm_internal_flash.h"
#include "wm_osal.h"
#include "wm_regs.h"

#include "n_delay.h"

#include "ws2812b.h"

#include "wm_gpio_afsel.h"
#include "wm_hostspi.h"
#include "wm_mem.h"

#define GREEN (0xff0000)
#define RED (0x00ff00)
#define BLUE (0x0000ff)
#define GREEN_SHIFT (16U)
#define RED_SHIFT (8U)

/*
Data transfer time( TH+TL=1.25 μs ±600n s )

0:
T0H 0 code ,high voltage time 0.4us ±150ns
T0L 0 code , low voltage time 0.85us ±150ns

1:
T1H 1 code ,high voltage time 0.85us ±150ns
T1L 1 code ,low voltage time 0.4us ±150ns

RES low voltage time Above 50μs
*/

static void
shift (const u16 offset, const u32 reg, const u8 pin, uint32_t data)
{
  for (int i = 23; i >= 0; i--)
    {

      if (((data >> i) & 0x01))
        { // 1
          tls_reg_write32 (HR_GPIO_DATA + offset,
                           reg | (1 << pin)); /* write high */
          n_delay_us (
              170); // freg 571.420  KHz CPU_CLK_240M     half period 0.85 us,
                    // + - значения чуть подравлены с учетом операторов в цикле
          tls_reg_write32 (HR_GPIO_DATA + offset,
                           reg & (~(1 << pin))); /* write low */
          n_delay_us (
              61); // freg 1.250009  MHz CPU_CLK_240M     half period 0.4 us
        }
      else
        { // 0
          tls_reg_write32 (HR_GPIO_DATA + offset,
                           reg | (1 << pin)); /* write high */
          n_delay_us (
              60); // freg 1.250009  MHz CPU_CLK_240M     half period 0.4 us
          tls_reg_write32 (HR_GPIO_DATA + offset,
                           reg & (~(1 << pin))); /* write low */
          n_delay_us (
              163); // freg 571.420  KHz CPU_CLK_240M     half period 0.85 us
        }
    }
}

static void
pin_mode_ws2812b_load_rgba (const ws2812b_t *dev, const color_rgba_t vals[])
{
  assert (dev && vals);

  tls_sys_clk sysclk;
  tls_sys_clk_get (&sysclk);
  if (sysclk.cpuclk != 240)
    {
      tls_sys_clk_set (
          CPU_CLK_240M); // нам мужно 240MHz, под это всё подогнано
    }

  u32 cpu_sr = 0;
  u32 reg;
  u32 reg_en;
  u8 pin;
  u16 offset;

  if (dev->data_pin
      >= WM_IO_PB_00) // w801 chip, only two GPIO port, PA (GPIOA - 16 bit) and
                      // PB (GPIOB - 32 bit), max power 12ma
    {
      pin = dev->data_pin - WM_IO_PB_00;
      offset = TLS_IO_AB_OFFSET;
    }
  else
    {
      pin = dev->data_pin;
      offset = 0;
    }

  cpu_sr = tls_os_set_critical (); // disable Interrupt !!!

  reg_en = tls_reg_read32 (HR_GPIO_DATA_EN + offset);
  tls_reg_write32 (HR_GPIO_DATA_EN + offset,
                   reg_en | (1 << pin)); // enabled control reg from need pin

  reg = tls_reg_read32 (HR_GPIO_DATA + offset); // load all pins from port

  for (int i = 0; i < dev->led_numof; i++)
    {
      uint32_t data = 0; // HEAD;
      /* we scale the 8-bit alpha value to a 5-bit value by cutting off the
       * 3 leas significant bits */
      switch ((u8)dev->rgb)
        {
        case WS_GRB_MODE:
          {
            data |= (((uint32_t)vals[i].color.g & (uint32_t)vals[i].alpha)
                     << GREEN_SHIFT);
            data |= (((uint32_t)vals[i].color.r & (uint32_t)vals[i].alpha)
                     << RED_SHIFT);
            data |= vals[i].color.b & (uint32_t)vals[i].alpha;
          };
          break;
        case WS_RGB_MODE:
          {
            data |= (((uint32_t)vals[i].color.r & (uint32_t)vals[i].alpha)
                     << GREEN_SHIFT);
            data |= (((uint32_t)vals[i].color.g & (uint32_t)vals[i].alpha)
                     << RED_SHIFT);
            data |= vals[i].color.b & (uint32_t)vals[i].alpha;
          };
          break;
        case WS_GBR_MODE:
          {
            data |= (((uint32_t)vals[i].color.g & (uint32_t)vals[i].alpha)
                     << GREEN_SHIFT);
            data |= (((uint32_t)vals[i].color.b & (uint32_t)vals[i].alpha)
                     << RED_SHIFT);
            data |= vals[i].color.r & (uint32_t)vals[i].alpha;
          };
          break;
        case WS_RBG_MODE:
          {
            data |= (((uint32_t)vals[i].color.r & (uint32_t)vals[i].alpha)
                     << GREEN_SHIFT);
            data |= (((uint32_t)vals[i].color.b & (uint32_t)vals[i].alpha)
                     << RED_SHIFT);
            data |= vals[i].color.g & (uint32_t)vals[i].alpha;
          };
          break;
        case WS_BGR_MODE:
          {
            data |= (((uint32_t)vals[i].color.b & (uint32_t)vals[i].alpha)
                     << GREEN_SHIFT);
            data |= (((uint32_t)vals[i].color.g & (uint32_t)vals[i].alpha)
                     << RED_SHIFT);
            data |= vals[i].color.r & (uint32_t)vals[i].alpha;
          };
          break;
        default:
          {
            data |= (((uint32_t)vals[i].color.b & (uint32_t)vals[i].alpha)
                     << GREEN_SHIFT);
            data |= (((uint32_t)vals[i].color.r & (uint32_t)vals[i].alpha)
                     << RED_SHIFT);
            data |= vals[i].color.g & (uint32_t)vals[i].alpha;
          };
          break;
        }

      shift (offset, reg, pin, data);
    }

  tls_reg_write32 (HR_GPIO_DATA + offset,
                   reg & (~(1 << pin))); /* write low from pin */

  tls_reg_write32 (HR_GPIO_DATA_EN + offset, reg_en); // reg_en return

  tls_os_release_critical (cpu_sr); // enable Interrupt

  // RES above 50μs
  n_delay_us (13000);

  if (sysclk.cpuclk != 240)
    {
      switch (sysclk.cpuclk) // восстанавливаем частоту
        {
        case 2:
          {
            tls_sys_clk_set (CPU_CLK_2M);
          };
          break;
        case 40:
          {
            tls_sys_clk_set (CPU_CLK_40M);
          };
          break;
        case 80:
          {
            tls_sys_clk_set (CPU_CLK_80M);
          };
          break;
        case 160:
          {
            tls_sys_clk_set (CPU_CLK_160M);
          };
          break;
        }
    }
}

#define MY_SPI_DATA_LEN 8192 // максимальный размер буфера для DMA = 8192 байт!

static char tx_buf[MY_SPI_DATA_LEN];

int
ws2812b_init (ws2812b_t *dev)
{

  assert (dev);

  switch ((int)dev->mode)
    {
    case WS_PIN_MODE:
      {
        printf ("ws2812b_init: PIN_MODE;\r\n");
        tls_gpio_cfg (dev->data_pin, WM_GPIO_DIR_OUTPUT,
                      WM_GPIO_ATTR_FLOATING);
        tls_gpio_write (dev->data_pin, 0);
      };
      break;
    case WS_I2S_MODE:
      {
        dev->data_pin = WM_IO_PB_11; // подключать к do, то есть к WM_IO_PB_11
        wm_i2s_ck_config (WM_IO_PB_08);// bit clock
        wm_i2s_ws_config (WM_IO_PB_09);//        0=left cannel,1=right cannel
        wm_i2s_di_config (WM_IO_PB_10);//
        wm_i2s_do_config (WM_IO_PB_11);// transmit  MSB -> LSB, MSB -> LSB, ...
        printf ("ws2812b_init: I2S configuration ck--PB08, ws--PB09, "
                "di--PB10, do--PB11;\r\n");
      };
      break;
    default: // or WS_SPI_MODE_Xbit
      {
        dev->data_pin = WM_IO_PB_17; // подключать к do, то есть к WM_IO_PB_17
        wm_spi_cs_config (WM_IO_PB_14);
        wm_spi_ck_config (WM_IO_PB_15);
        wm_spi_di_config (WM_IO_PB_16);
        wm_spi_do_config (WM_IO_PB_17);
        printf ("ws2812b_init: MASTER SPI configuratioin cs--PB14, ck--PB15, "
                "di--PB16, do--PB17;\r\n");
        tls_spi_trans_type (SPI_DMA_TRANSFER);
        /*
                1,25 µs	=	800000 Hz
                0,3   µs	=	3333333.3333333 Hz
                0.31  µs	=	3225806.4516129 Hz
                0.313	=	3194888.1789137 Hz
                                                                                LED =
        RGB =	SPI tx =	bit =	буффер, байт =
        SPI ?	4 имп по 0,3 мкс					нужно 4
        бита
        на каждый бит			60	24	4	5760 720 0,15
        24	4	12 0,31	0,31	0,31	0,31	1,24
        60			12	720 0,3	0,3	0,3	0,3	1,2

        bit
        0	0,35		0,9		1,25

        1		0,9		0,35

        Если мы передаём ноль, то сначала мы устанавливаем высокий уровень
        сигнала, держим его в таком состоянии 0,35 микросекунды (допускается
        отклонение 150 наносекунд или 0,15 микросекунды). По прошествии данного
        времени мы устанавливаем на ножке низкий уровень и держим его до
        передачи следующего бита 0,9 микросекунды (допускается такое же
        отклонение — 150 наносекунд)

        Единица передаётся наоборот. Мы также устанавливаем высокий уровень,
        ждём 0,9 микросекунды (отклонение то же), затем опускаем уровень, ждём
        0,35 микросекунды (отклонение такое же — 150 наносекунд)

        вот, такая-же идея (почти, до табл, перекодировки я не додумалься)
        https://michaeltien8901.github.io/stm32/2018/07/19/Using-STM32-SPI-For-LED-STRIP.html
        git clone
        https://bitbucket.org/mtien888/stm32-for-ws2812b-using-spi/src/
        .\neopixel

        apbclk 		SPI
        MHz     div     MHz             KHz		Hz
        40	2	20		20000		20000000
        40	4	10		10000		10000000
        40	8	5		5000		5000000
        40	16	2,5		2500		2500000
        40	32	1,25		1250		1250000
        40	64	0,625		625		625000
        40	128	0,3125		312,5		312500
        40	256	0,15625		156,25		156250
        40	512	0,078125	78,125		78125
        40	1024	0,0390625	39,0625		39062,5
        40	2048	0,01953125	19,53125	19531,25
        40	4096	0,009765625	9,765625	9765,625
        40	8192	0,004882813	4,8828125	4882,8125
        40	16384	0,002441406	2,44140625	2441,40625
        40	32768	0,001220703	1,220703125	1220,703125
        40	40000	0,001		1		1000
        может еще возможно делители, то что кратно 10... 1000 Hz же есть!

         ?A reset code should be sent before and after sending the data?
        https://www.hackster.io/RVLAD/neopixel-ws2812b-spi-driver-with-ada-on-stm32f4-discovery-d330ea
        тут использовали spi, 1 байт на 1 бит
        передача 0 это 0хC0 = 0x11000000b
        передача 1 это 0хF8 = 0x11111000b
        reset передача 0x00 = 0x00000000b

        это на частоте 8 * 800 Kbit/s = 6.4 Mbit/s
        */
        switch ((u8)dev->mode)
          {
          case WS_SPI_MODE_3bit:
            {
              dev->spi_clk = 2000000;
              dev->spi_on_bit = 2;
              dev->spi_off_bit = 1;
            };
            break;
          case WS_SPI_MODE_4bit:
            {
              dev->spi_clk = 3265000;
              dev->spi_on_bit = 3;
              dev->spi_off_bit = 1;
            };
            break;
          case WS_SPI_MODE_5bit:
            {
              dev->spi_clk = 4000000;
              dev->spi_on_bit = 4;
              dev->spi_off_bit = 2;
            };
            break;
          case WS_SPI_MODE_6bit:
            {
              dev->spi_clk = 4800000;
              dev->spi_on_bit = 4;
              dev->spi_off_bit = 2;
            };
            break;
          case WS_SPI_MODE_7bit:
            {
              dev->spi_clk = 5600000;
              dev->spi_on_bit = 5;
              dev->spi_off_bit = 2;
            };
            break;
          case WS_SPI_MODE_8bit:
            {
              dev->spi_clk = 6400000;
              dev->spi_on_bit = 5;
              dev->spi_off_bit = 2;
            };
            break;
          }
        /**
         * @brief          This function is used to setup the spi controller.
         *
         * @param[in]      mode         is CPOL and CPHA type defined in
         * TLS_SPI_MODE_0 to TLS_SPI_MODE_3
         * @param[in]      cs_active    is cs mode, defined as TLS_SPI_CS_LOW
         * or TLS_SPI_CS_HIGH
         * @param[in]      fclk            is spi clock,the unit is HZ.
         *
         * @retval         TLS_SPI_STATUS_OK			if setup
         * success
         * @retval         TLS_SPI_STATUS_EMODENOSUPPORT	if mode is not
         * support
         * @retval         TLS_SPI_STATUS_EINVAL			if
         * cs_active is not support
         * @retval         TLS_SPI_STATUS_ECLKNOSUPPORT	if fclk is not support
         *
         * @note           None
         */
        switch (tls_spi_setup (TLS_SPI_MODE_0, TLS_SPI_CS_LOW, dev->spi_clk))
          {
          case TLS_SPI_STATUS_OK:
            {
              printf ("tls_spi_setup TLS_SPI_STATUS_OK\n");
            };
            break;
          case TLS_SPI_STATUS_EMODENOSUPPORT:
            {
              printf ("tls_spi_setup TLS_SPI_STATUS_EMODENOSUPPORT\n");
              return WM_FAILED;
            };
            break;
          case TLS_SPI_STATUS_EINVAL:
            {
              printf ("tls_spi_setup TLS_SPI_STATUS_EINVAL\n");
              return WM_FAILED;
            };
            break;
          case TLS_SPI_STATUS_ECLKNOSUPPORT:
            {
              printf ("tls_spi_setup TLS_SPI_STATUS_ECLKNOSUPPORT\n");
              return WM_FAILED;
            };
            break;
          }
      };
      break;
    }

  return WM_SUCCESS;
}

static void i2s_mode_ws2812b_load_rgba (const ws2812b_t *dev,
                                        const color_rgba_t vals[]);

void
ws2812b_load_rgba (const ws2812b_t *dev, const color_rgba_t vals[])
{
  assert (dev && vals);

  u8 u8_bit = 8;
  switch ((u8)dev->mode)
    {
    case WS_PIN_MODE:
      {
        pin_mode_ws2812b_load_rgba (dev, vals);
        return;
      };
      break;
    case WS_I2S_MODE:
      {
        i2s_mode_ws2812b_load_rgba (dev, vals);
        return;
      };
      break;
    case WS_SPI_MODE_3bit:
      {
        u8_bit = 3;
      };
      break;
    case WS_SPI_MODE_4bit:
      {
        u8_bit = 4;
      };
      break;
    case WS_SPI_MODE_5bit:
      {
        u8_bit = 5;
      };
      break;
    case WS_SPI_MODE_6bit:
      {
        u8_bit = 6;
      };
      break;
    case WS_SPI_MODE_7bit:
      {
        u8_bit = 7;
      };
      break;
    case WS_SPI_MODE_8bit:
      {
        u8_bit = 8;
      };
      break;
    }

  memset (tx_buf, 0, MY_SPI_DATA_LEN);
  u16 iPosBit = 8 * 3; // reset code? в начале.... ну допустим, 8*3, три байта

  u8 spi_on_bit = dev->spi_on_bit;
  u8 spi_off_bit = dev->spi_off_bit;

  if (spi_on_bit > u8_bit)
    spi_on_bit = u8_bit;
  if (spi_off_bit > u8_bit)
    spi_off_bit = u8_bit;

  for (int i = 0; i < dev->led_numof; i++)
    {
      uint32_t data = 0; // HEAD;

      switch ((u8)dev->rgb)
        {
        case WS_GRB_MODE:
          {
            data |= (((uint32_t)vals[i].color.g & (uint32_t)vals[i].alpha)
                     << GREEN_SHIFT);
            data |= (((uint32_t)vals[i].color.r & (uint32_t)vals[i].alpha)
                     << RED_SHIFT);
            data |= vals[i].color.b & (uint32_t)vals[i].alpha;
          };
          break;
        case WS_RGB_MODE:
          {
            data |= (((uint32_t)vals[i].color.r & (uint32_t)vals[i].alpha)
                     << GREEN_SHIFT);
            data |= (((uint32_t)vals[i].color.g & (uint32_t)vals[i].alpha)
                     << RED_SHIFT);
            data |= vals[i].color.b & (uint32_t)vals[i].alpha;
          };
          break;
        case WS_GBR_MODE:
          {
            data |= (((uint32_t)vals[i].color.g & (uint32_t)vals[i].alpha)
                     << GREEN_SHIFT);
            data |= (((uint32_t)vals[i].color.b & (uint32_t)vals[i].alpha)
                     << RED_SHIFT);
            data |= vals[i].color.r & (uint32_t)vals[i].alpha;
          };
          break;
        case WS_RBG_MODE:
          {
            data |= (((uint32_t)vals[i].color.r & (uint32_t)vals[i].alpha)
                     << GREEN_SHIFT);
            data |= (((uint32_t)vals[i].color.b & (uint32_t)vals[i].alpha)
                     << RED_SHIFT);
            data |= vals[i].color.g & (uint32_t)vals[i].alpha;
          };
          break;
        case WS_BGR_MODE:
          {
            data |= (((uint32_t)vals[i].color.b & (uint32_t)vals[i].alpha)
                     << GREEN_SHIFT);
            data |= (((uint32_t)vals[i].color.g & (uint32_t)vals[i].alpha)
                     << RED_SHIFT);
            data |= vals[i].color.r & (uint32_t)vals[i].alpha;
          };
          break;
        default:
          {
            data |= (((uint32_t)vals[i].color.b & (uint32_t)vals[i].alpha)
                     << GREEN_SHIFT);
            data |= (((uint32_t)vals[i].color.r & (uint32_t)vals[i].alpha)
                     << RED_SHIFT);
            data |= vals[i].color.g & (uint32_t)vals[i].alpha;
          };
          break;
        }

      for (int i = 23; i >= 0; i--)
        {
          if (((data >> i) & 0x01))
            { // 1 -> 111100
              int iByte = 0;
              for (int x = 0; x < spi_on_bit; x++)
                {
                  iByte = iPosBit / 8;
                  u8 iBit = iPosBit - (iByte * 8);
                  tx_buf[iByte] |= (1 << (u8_bit - iBit));
                  iPosBit += 1;
                }
              iPosBit += (u8_bit - spi_on_bit);
            }
          else
            { // 0 -> 110000      или 0 -> 100000
              int iByte = 0;

              for (int x = 0; x < spi_off_bit; x++)
                {
                  iByte = iPosBit / 8;
                  u8 iBit = iPosBit - (iByte * 8);
                  tx_buf[iByte] |= (1 << (u8_bit - iBit));
                  iPosBit += 1;
                }
              iPosBit += (u8_bit - spi_off_bit);
            }

          if ((iPosBit / 8) > (MY_SPI_DATA_LEN - 10))
            break;
        }
    }

  iPosBit += 8 * 3;
  //   printf("SPI Master send %d byte, modeA, little endian\n",
  //   MY_SPI_DATA_LEN);
  /*
  Для 3 бит
         частота 2399999.99808Hz
  1.25/3=0,416666667
    100
  0,416666667*1=0,416666667
    111
  0,416666667*3=0,833333333
  MAX_LED=8192/3/3=910

  Для 4 бит
         частота 3200000Hz
  1.25/4=0.3125
    1000
  0.3125*1=0.3125
    1110
  0.3125*3=0.937
  MAX_LED=8192/4/3=682

  Для 5 бит
         частота 4000000Hz
  1.25/5=0.25
    11000
  0.25*2=0.5
    11100
  0.25*3=0.75
  MAX_LED=8192/5/3=546

  Для 6 бит
         частота 4800000.00768Hz
  1.25/6=0,208333333
    110000
  0,208333333*2=0,416666667
    111100
  0,208333333*4=0,833333333
  MAX_LED=8192/6/3=453

  Для 7 бит - не очень подходит
         частота 5599999.98656Hz
  1.25/7=0,178571429
    1100000
  0,178571429*2=0,357142857
    1111000
  0,178571429*4=0,714285714
  MAX_LED=8192/7/3=390

  Для 8 бит - работает, это еще с учетом того что частота нужна 6Мгц, и там
  первый бит чуть дольше, что плюс частота 6400000Hz 1.25/8=0,15625 надо 1.25
  μs ±600n   0,65 1,25 1,85 11000000 0,15625*2=0,3125   надо 0,4us ±150ns 0,25
  0,4 0,55 11111000 0,15625*5=0.78125  надо 0,85us ±150ns   0,7 0,85 1,0
  MAX_LED=8192/8/3=341

  Data transfer time( TH+TL=1.25 μs ±600n s )
   0:
  T0H 0 code ,high voltage time 0.4us ±150ns
  T0L 0 code , low voltage time 0.85us ±150ns
   1:
  T1H 1 code ,high voltage time 0.85us ±150ns
  T1L 1 code ,low voltage time 0.4us ±150ns

  RES low voltage time Above 50μs
  */
  switch (tls_spi_write ((u8 *)tx_buf, (iPosBit / 8)))
    {
    case TLS_SPI_STATUS_OK:
      {
        // printf("tls_spi_write TLS_SPI_STATUS_OK\n");
      };
      break;
    case TLS_SPI_STATUS_EINVAL:
      {
        printf ("tls_spi_write TLS_SPI_STATUS_EINVAL\n");
        //        return WM_FAILED;
      };
      break;
    case TLS_SPI_STATUS_ENOMEM:
      {
        printf ("tls_spi_write TLS_SPI_STATUS_ENOMEM\n");
        //        return WM_FAILED;
      };
      break;
    case TLS_SPI_STATUS_ESHUTDOWN:
      {
        printf ("tls_spi_write TLS_SPI_STATUS_ESHUTDOWN\n");
        //        return WM_FAILED;
      };
      break;
    }

  // RES above 50μs
  n_delay_us (500);
}

/** Use this one if you are using WS2811 or WS2812 neopixels */
#define LED_TYPE_WS2812 0

/** Use this one if you are using the SK6812 type neopixels */
#define LED_TYPE_SK6812 1

/** define this macro on the cmdline (-DLED_TYPE=) to select
 *  which timings to use */
#ifndef LED_TYPE
#define LED_TYPE LED_TYPE_WS2812
#endif

#if LED_TYPE == LED_TYPE_WS2812
static const int16_t bitpatterns[16] = {
  0b1000100010001000, 0b1000100010001110, 0b1000100011101000,
  0b1000100011101110, 0b1000111010001000, 0b1000111010001110,
  0b1000111011101000, 0b1000111011101110, 0b1110100010001000,
  0b1110100010001110, 0b1110100011101000, 0b1110100011101110,
  0b1110111010001000, 0b1110111010001110, 0b1110111011101000,
  0b1110111011101110,
};
#endif

#if LED_TYPE == LED_TYPE_SK6812
static const uint16_t bitpatterns[16] = {
  0b1000100010001000, 0b1000100010001100, 0b1000100011001000,
  0b1000100011001100, 0b1000110010001000, 0b1000110010001100,
  0b1000110011001000, 0b1000110011001100, 0b1100100010001000,
  0b1100100010001100, 0b1100100011001000, 0b1100100011001100,
  0b1100110010001000, 0b1100110010001100, 0b1100110011001000,
  0b1100110011001100,
};
#endif

/** Your LEDs have three color components */
#define LED_MODE_RGB 3

/** Your LEDs have four color components */
#define LED_MODE_RGBW 4

/** Select how many color components are in your pixels by
 *  defining it on the cmdline (-DLED_MODE=) */
#ifndef LED_MODE
#define LED_MODE LED_MODE_RGB
#endif

/*
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
#if LED_MODE == LED_MODE_RGBW
    uint8_t white;
#endif
} ws2812_pixel_t;
*/

static I2S_InitDef i2s_config
    = { I2S_MODE_MASTER, I2S_CTRL_MONO,     I2S_RIGHT_CHANNEL,
        I2S_Standard,    I2S_DataFormat_16, 8000,
        5000000 };

#define UNIT_SIZE 2 * 4096 //1024
#define PCM_ADDRDSS 0x100000
#define FIRM_SIZE 940

int16_t data_1[2][UNIT_SIZE];

void
i2s_demo_callback_play (uint32_t *data, uint16_t *len)
{
/*
  static int number = 0;

  tls_fls_read (PCM_ADDRDSS + number * UNIT_SIZE * 2, (u8 *)data,
                UNIT_SIZE * 2);

  number++;
  if (number > FIRM_SIZE / UNIT_SIZE / 1024 / 2)
    {
      number = 0;
      *len = 0xFFFF;
    }
  printf ("%d, %x\n", number, data[0]);
*/
printf ("i2s_demo_callback_play data[0] =  %x\n", data[0]);
}

static void
i2s_mode_ws2812b_load_rgba (const ws2812b_t *dev, const color_rgba_t vals[])
{
  assert (dev && vals);

  memset (data_1[0], 0, UNIT_SIZE);
  memset (data_1[1], 0, UNIT_SIZE);

  int16_t *p_dma_buf = data_1[0];

  wm_i2s_port_init (&i2s_config);
  wm_i2s_register_callback (i2s_demo_callback_play);

  for (int i = 0; i < dev->led_numof; i++)
    {

      // green
      *p_dma_buf++ = bitpatterns[vals[i].color.g & 0x0F];
      *p_dma_buf++ = bitpatterns[vals[i].color.g >> 4];

      // red
      *p_dma_buf++ = bitpatterns[vals[i].color.r & 0x0F];
      *p_dma_buf++ = bitpatterns[vals[i].color.r >> 4];

      // blue
      *p_dma_buf++ = bitpatterns[vals[i].color.b & 0x0F];
      *p_dma_buf++ = bitpatterns[vals[i].color.b >> 4];

#if LED_MODE == LED_MODE_RGBW
      // white
      *p_dma_buf++ = bitpatterns[vals[i].alpha & 0x0F];
      *p_dma_buf++ = bitpatterns[vals[i].alpha >> 4];
#endif
    }

  wm_i2s_tx_rx_dma (&i2s_config, data_1[0], data_1[1], UNIT_SIZE);
  wm_i2s_tx_rx_stop ();
}
