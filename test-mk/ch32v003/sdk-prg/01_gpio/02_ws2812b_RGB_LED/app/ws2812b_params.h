/**
 * @ingroup     drivers_ws2812b
 * @{
 *
 * @file
 * @brief       WS2812B board specific configuration
 *
 */

#ifndef WS2812B_PARAMS_H
#define WS2812B_PARAMS_H

//#include "board.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief   Set default configuration parameters for the WS2812B driver
 * @{
 */
#ifndef WS2812B_PARAM_LED_NUMOF
#define WS2812B_PARAM_LED_NUMOF (5) /* many have 60 per meter... */
#endif
#ifndef WS2812B_PARAM_DATA_PIN
#define WS2812B_PARAM_DATA_PIN (PA_01)
#endif

#ifndef WS2812B_PARAMS
#define WS2812B_PARAMS                                                        \
  {                                                                           \
    .led_numof = WS2812B_PARAM_LED_NUMOF, .data_pin = WS2812B_PARAM_DATA_PIN  \
  }
#endif
  /**@}*/

  /**
   * @brief   WS2812B configuration
   */
  static const ws2812b_params_t ws2812b_params[] = { WS2812B_PARAMS };

#ifdef __cplusplus
}
#endif

#endif /* WS2812B_PARAMS_H */
/** @} */
