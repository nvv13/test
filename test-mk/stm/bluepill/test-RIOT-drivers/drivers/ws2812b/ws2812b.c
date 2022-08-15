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
 * @brief       WS2812B RGB LED driver implementation
 *
 *
 * @}
 */

#include <string.h>

#include "assert.h"
#include "ws2812b.h"

#define BLUE            (0xff0000)
#define GREEN           (0x00ff00)
#define RED             (0x0000ff)
#define BLUE_SHIFT      (16U)
#define GREEN_SHIFT     (8U)


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

static inline void shift(const ws2812b_t *dev, uint32_t data)
{
    for (int i = 23; i >= 0; i--) {
        if( ((data >> i) & 0x01) )
        {//1
        gpio_set(dev->data_pin);// gpio_write(dev->data_pin, 0x01);//T1H 1 code ,high voltage time 0.85us ±150ns
        gpio_set(dev->data_pin);//gpio_write(dev->data_pin, 0x01);//T1H 1 code ,high voltage time 0.85us ±150ns
        gpio_set(dev->data_pin);//gpio_write(dev->data_pin, 0x01);//T1H 1 code ,high voltage time 0.85us ±150ns
        gpio_clear(dev->data_pin);//gpio_write(dev->data_pin, 0x00);//T1L 1 code ,low voltage time 0.4us ±150ns
        }
        else
        {//0
        gpio_set(dev->data_pin);//gpio_write(dev->data_pin, 0x01);//T0H 0 code ,high voltage time 0.4us ±150ns
        gpio_clear(dev->data_pin);//gpio_write(dev->data_pin, 0x00);//T0L 0 code , low voltage time 0.85us ±150ns
        gpio_clear(dev->data_pin);//gpio_write(dev->data_pin, 0x00);//T0L 0 code , low voltage time 0.85us ±150ns
        gpio_clear(dev->data_pin);//gpio_write(dev->data_pin, 0x00);//T0L 0 code , low voltage time 0.85us ±150ns
        }
    }
}

void ws2812b_init(ws2812b_t *dev, const ws2812b_params_t *params)
{
    assert(dev && params);

    *dev = *params;

    gpio_init(dev->data_pin, GPIO_OUT);
    gpio_clear(dev->data_pin);
}

void ws2812b_load_rgba(const ws2812b_t *dev, const color_rgba_t vals[])
{
    assert(dev && vals);

    for (int i = 0; i < dev->led_numof; i++) {
        uint32_t data = 0;//HEAD;
        /* we scale the 8-bit alpha value to a 5-bit value by cutting off the
         * 3 leas significant bits */
        data |= (((uint32_t)vals[i].color.b & (uint32_t)vals[i].alpha)<< BLUE_SHIFT);
        data |= (((uint32_t)vals[i].color.g & (uint32_t)vals[i].alpha )<< GREEN_SHIFT);
        data |= vals[i].color.r & (uint32_t)vals[i].alpha;
        shift(dev, data);
    }
    gpio_write(dev->data_pin, 0x00);//RES low voltage time Above 50μs
    gpio_clear(dev->data_pin);
    gpio_clear(dev->data_pin);
    gpio_clear(dev->data_pin);
}
