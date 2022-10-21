/**
 * @defgroup    drivers_ws2812b WS2812B RGB LED
 * @ingroup     drivers_actuators
 * @brief       Driver for chained WS2812B RGB LEDs
 * @{
 *
 * @file
 * @brief       Interface for controlling WS2812B LEDs
 *
 * @author      nvv13
 */

#ifndef WS2812B_H
#define WS2812B_H

#include "color.h"
#include "wm_gpio.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * @brief   Configuration parameters for (chained) WS2812B LEDs
   */

  typedef enum ws2812b_mode
  {
    WS_PIN_MODE = 0,
    /* SPI_MODE тут только с DMA, а в этом режиме, буфер ораничен 8192 байт */
    /* на каждый LED нужно 24 бит, он кодируется 3...8 бит в SPI_MODE */
    /* значит количество LEDs ограничено (+6 байт на вх вых режим типа)*/
    WS_SPI_MODE_3bit = 1, /* ((8192-6)*8)/3/24=909 */
    WS_SPI_MODE_4bit = 2, /* ((8192-6)*8)/4/24=682 */
    WS_SPI_MODE_5bit = 3, /* ((8192-6)*8)/5/24=545 */
    WS_SPI_MODE_6bit = 4, /* ((8192-6)*8)/6/24=454 */
    WS_SPI_MODE_7bit = 5, /* ((8192-6)*8)/3/24=389 */
    WS_SPI_MODE_8bit = 6  /* ((8192-6)*8)/3/24=341 */
  } _ws2812b_mode;

  typedef enum rgb_mode
  {
    WS_RGB_MODE = 0,
    WS_GRB_MODE = 1,
    WS_GBR_MODE = 2,
    WS_RBG_MODE = 3,
    WS_BGR_MODE = 4,
  } _rgb_mode;

  typedef struct
  {
    u16 led_numof;             /**< number of chained LEDs */
    enum tls_io_name data_pin; /**< data pin - always 17 */
    enum ws2812b_mode mode;
    u32 spi_clk; /* частота для режимов SPI */
    u8 spi_on_bit; /* сколько бит на передачу 1 , при смене mode -
                      устанавливаеться автоматически */
    u8 spi_off_bit; /* сколько бит на передачу 0 , а потом, можно поменять для
                       пробы типа */
    enum rgb_mode rgb;
  } ws2812b_params_t;

  /**
   * @brief   Device descriptor definition for WS2812B LEDs
   */
  typedef ws2812b_params_t ws2812b_t;

  /**
   * @brief   Initialize (chained) WS2812B LEDs
   *
   * @param[out] dev      device descriptor
   * @param[in]  params   device configuration
   *
   * @pre     @p dev != NULL
   * @pre     @p params != NULL
   */
  int ws2812b_init (ws2812b_t *dev);

  /**
   * @brief   Apply the given color values to the connected LED(s)
   *
   * @param[in] dev       device descriptor
   * @param[in] vals      color values, MUST be of size `dev->led_numof`
   *
   * @pre     @p dev != NULL
   * @pre     @p vals != NULL
   */
  void ws2812b_load_rgba (const ws2812b_t *dev, const color_rgba_t vals[]);

#ifdef __cplusplus
}
#endif

#endif /* WS2812B_H */
/** @} */
