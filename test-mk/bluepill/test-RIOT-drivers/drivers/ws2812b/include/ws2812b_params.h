/*
 * Copyright (C) 2017 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

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

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Set default configuration parameters for the WS2812B driver
 * @{
 */
#ifndef WS2812B_PARAM_LED_NUMOF
#define WS2812B_PARAM_LED_NUMOF      (60)    /* many have 60 per meter... */
#endif
#ifndef WS2812B_PARAM_DATA_PIN
#define WS2812B_PARAM_DATA_PIN       (GPIO_PIN(0, 0))
#endif

#ifndef WS2812B_PARAMS
#define WS2812B_PARAMS               { .led_numof = WS2812B_PARAM_LED_NUMOF, \
                                       .data_pin  = WS2812B_PARAM_DATA_PIN  }
#endif
/**@}*/

/**
 * @brief   WS2812B configuration
 */
static const  ws2812b_params_t ws2812b_params[] =
{
    WS2812B_PARAMS
};

#ifdef __cplusplus
}
#endif

#endif /* WS2812B_PARAMS_H */
/** @} */
