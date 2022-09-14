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
#include "wm_osal.h"
#include "wm_regs.h"

//#include "csi_core.h"
// extern uint32_t csi_coret_get_load (void);
// extern uint32_t csi_coret_get_value (void);

#include "n_delay.h"

#include "ws2812b.h"

#include "wm_gpio_afsel.h"
#include "wm_hostspi.h"
#include "wm_mem.h"
#include "ws2812b_params.h"
#define MY_SPI_DATA_LEN                                                       \
  (WS2812B_PARAM_LED_NUMOF * 12) // num LED * 24 * 4 / 8 = buf SPI
/*
                                    один LED это, R G B - три светодиода по 8 бит,
                                    8*3=24 бит, значит нужен буфер 24*колич.LED бит
                                    каждый бит предается 4 витами по SPI 
                                    потом это разделит на 8 (в 1 байте буфура 8 бит)
*/                                       
#define BLUE (0xff0000)
#define GREEN (0x00ff00)
#define RED (0x0000ff)
#define BLUE_SHIFT (16U)
#define GREEN_SHIFT (8U)

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

static char *tx_buf = NULL;

int
ws2812b_init (ws2812b_t *dev, const ws2812b_params_t *params)
{
  assert (dev && params);

  *dev = *params;

  /*MASTER SPI configuratioin*/
  wm_spi_cs_config (WM_IO_PB_14);
  wm_spi_ck_config (WM_IO_PB_15);
  wm_spi_di_config (WM_IO_PB_16);
  wm_spi_do_config (WM_IO_PB_17);
  printf (
      "MASTER SPI configuratioin cs--PB14, ck--PB15, di--PB16, do--PB17;\r\n");
  // подключать к do, то есть к WM_IO_PB_17

  /*
          1,25 µs	=	800000 Hz
          0,3   µs	=	3333333.3333333 Hz
          0.31  µs	=	3225806.4516129 Hz
          0.313	=	3194888.1789137 Hz
                                                                          LED =
  RGB =	SPI tx =	bit =	буффер, байт =
  SPI ?	4 имп по 0,3 мкс					нужно 4 бита на
  каждый бит			60	24	4	5760	720 0,15
  24	4	12 0,31	0,31	0,31	0,31	1,24
  60			12	720 0,3	0,3	0,3	0,3	1,2

  bit
  0	0,35		0,9		1,25

  1		0,9		0,35


  Если мы передаём ноль, то сначала мы устанавливаем высокий уровень сигнала,
  держим его в таком состоянии 0,35 микросекунды (допускается отклонение 150
  наносекунд или 0,15 микросекунды). По прошествии данного времени мы
  устанавливаем на ножке низкий уровень и держим его до передачи следующего
  бита 0,9 микросекунды (допускается такое же отклонение — 150 наносекунд)

  Единица передаётся наоборот. Мы также устанавливаем высокий уровень, ждём 0,9
  микросекунды (отклонение то же), затем опускаем уровень, ждём 0,35
  микросекунды (отклонение такое же — 150 наносекунд)

  вот, такая-же идея (почти, до табл, перекодировки я не додумалься)
  https://michaeltien8901.github.io/stm32/2018/07/19/Using-STM32-SPI-For-LED-STRIP.html
  git clone https://bitbucket.org/mtien888/stm32-for-ws2812b-using-spi/src/
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


  https://www.hackster.io/RVLAD/neopixel-ws2812b-spi-driver-with-ada-on-stm32f4-discovery-d330ea
  тут использовали spi, 1 байт на 1 бит
  передача 0 это 0хC0 = 0x11000000b
  передача 1 это 0хF8 = 0x11111000b
  reset передача 0x00 = 0x00000000b

  это на частоте 8 * 800 Kbit/s = 6.4 Mbit/s



  int master_spi_send_data(int clk, int type)
  {
  */

  //int clk = 3225806;          /* default 1M */
  //int clk = 4000000; /* default 1M */
  int clk = 3000000; /* default 1M */
  tls_spi_trans_type (SPI_DMA_TRANSFER);

  /**
   * @brief          This function is used to setup the spi controller.
   *
   * @param[in]      mode         is CPOL and CPHA type defined in
   * TLS_SPI_MODE_0 to TLS_SPI_MODE_3
   * @param[in]      cs_active    is cs mode, defined as TLS_SPI_CS_LOW or
   * TLS_SPI_CS_HIGH
   * @param[in]      fclk            is spi clock,the unit is HZ.
   *
   * @retval         TLS_SPI_STATUS_OK			if setup success
   * @retval         TLS_SPI_STATUS_EMODENOSUPPORT	if mode is not support
   * @retval         TLS_SPI_STATUS_EINVAL			if cs_active is
   * not support
   * @retval         TLS_SPI_STATUS_ECLKNOSUPPORT	if fclk is not support
   *
   * @note           None
   */

  switch (tls_spi_setup (TLS_SPI_MODE_0, TLS_SPI_CS_LOW, clk))
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

  tx_buf = tls_mem_alloc (MY_SPI_DATA_LEN);
  if (NULL == tx_buf)
    {
      printf ("\nspi_demo tx mem err\n");
      return WM_FAILED;
    }

  // memset(tx_buf,  0, MY_SPI_DATA_LEN);
  /*
      strcpy(tx_buf, "data");
      p = (int *)&tx_buf[4];
      *p = 1500;
      p ++;
      for(i = 0;i < (MY_SPI_DATA_LEN-8)/4;i ++)
      {
          *p = 0x12345678;
           p ++;
      }
  */

  //    tls_mem_free(tx_buf);

  //    printf("after send\n");
  return WM_SUCCESS;

  //    tls_gpio_cfg(dev->data_pin, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  //    tls_gpio_write(dev->data_pin, 0);
}

void
ws2812b_load_rgba (const ws2812b_t *dev, const color_rgba_t vals[])
{
  assert (dev && vals);

  memset (tx_buf, 0, MY_SPI_DATA_LEN);

  int iPosBit = 0;

  for (int i = 0; i < dev->led_numof; i++)
    {
      uint32_t data = 0; // HEAD;
      /* we scale the 8-bit alpha value to a 5-bit value by cutting off the
       * 3 leas significant bits */
      data |= (((uint32_t)vals[i].color.b & (uint32_t)vals[i].alpha)
               << BLUE_SHIFT);
      data |= (((uint32_t)vals[i].color.g & (uint32_t)vals[i].alpha)
               << GREEN_SHIFT);
      data |= vals[i].color.r & (uint32_t)vals[i].alpha;

      data = 0;
      //        shift(offset, reg, pin, data);
      for (int i = 23; i >= 0; i--)
        {

          if (((data >> i) & 0x01))
            { // 1 -> 1110
              int iByte = 0;
              if (iPosBit > 0)
                iByte = iPosBit / 8;
              int iBit = iPosBit - (iByte * 8);
              tx_buf[iByte]
                  |= ((1 << iBit) | (1 << (iBit + 1)) | (1 << (iBit + 2)));
              iPosBit += 4;
            }
          else
            { // 0 -> 1000
              int iByte = 0;
              if (iPosBit > 0)
                iByte = iPosBit / 8;
              int iBit = iPosBit - (iByte * 8);
              tx_buf[iByte] |= ((1 << iBit));
              iPosBit += 4;
            }
        }
    }

  //   printf("SPI Master send %d byte, modeA, little endian\n",
  //   MY_SPI_DATA_LEN);
  switch (tls_spi_write ((u8 *)tx_buf, MY_SPI_DATA_LEN))
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
  n_delay_us (50);
}
