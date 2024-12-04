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
#include "port.h"

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * @brief   Configuration parameters for (chained) WS2812B LEDs
   */
  typedef struct
  {
    int led_numof;  /**< number of chained LEDs */
    pin_t data_pin; /**< data pin */
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
  void ws2812b_init (ws2812b_t *dev, const ws2812b_params_t *params);

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
