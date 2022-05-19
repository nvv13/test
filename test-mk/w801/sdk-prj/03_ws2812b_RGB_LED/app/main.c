/***************************************************************************** 
* 
* File Name : main.c
* 
* Description: main 
*
* Autor : nvv13
* 
* Date : 2022-05-19
*****************************************************************************/ 

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include "wm_type_def.h"
//#include "wm_uart.h"
#include "wm_gpio.h"
//#include "wm_hostspi.h"
//#include "wm_socket.h"
//#include "wm_sockets.h"
//#include "wm_wifi.h"
//#include "wm_hspi.h"
//#include "wm_pwm.h"
//#include "wm_params.h"
#include "wm_osal.h"
//#include "wm_netif.h"
//#include "wm_efuse.h"
//#include "wm_mem.h"
//#include "wm_regs.h"

#include "ws2812b.h"
#include "ws2812b_params.h"


#define    DEMO_TASK_SIZE      2048
static OS_STK 			DemoTaskStk[DEMO_TASK_SIZE];
#define  DEMO_TASK_PRIO			                32



#define US_PER_MS	10

/**
 * @brief   Switch to the next LED every 10ms
 */
#define STEP        (200 * US_PER_MS)

/**
 * @brief   Interval for dimming colors
 */
#define DIM         (100 * US_PER_MS)

/**
 * @brief   Step through brightness levels (0-255) in 32 steps
 */
#define BSTEP       (8U)

/**
 * @brief   Allocate the device descriptor
 */
static ws2812b_t dev;

/**
 * @brief   Allocate a color_rgb_t struct for each LED on the strip
 */
static color_rgba_t leds[WS2812B_PARAM_LED_NUMOF];

static void setcolor(int color, uint8_t alpha)
{
    for (int i = 0; i < (int)WS2812B_PARAM_LED_NUMOF; i++) {
        leds[i].alpha = alpha;
        memset(&leds[i].color, 0, sizeof(color_rgb_t));
        switch (color) {
            case 0:
                leds[i].color.r = 255;
                break;
            case 1:
                leds[i].color.g = 255;
                break;
            case 2:
                leds[i].color.b = 255;
                break;
        }
    }
}


void demo_console_task(void *sdata)
{

    int pos = 0;
    int step = 1;
    color_hsv_t col = { 0.0, 1.0, 1.0 };

    puts("WS2812B Test App");

    /* initialize all LED color values to black (off) */
    memset(leds, 0, sizeof(color_rgba_t) * WS2812B_PARAM_LED_NUMOF);


    ws2812b_params_t param;
    param.led_numof = 10;
    param.data_pin  = WM_IO_PB_01;

    ws2812b_init(&dev, &param );

    puts("Initialization done.");

    while (1) {
    setcolor(-1, 255);
    ws2812b_load_rgba(&dev, leds);
}

    /* set to each red, green, and blue, and fade each color in and out */
    for (int col = 0; col <= 2; col++) {
        int i = 0;
        for (; i <= (int)UINT8_MAX; i += BSTEP) {
            setcolor(col, (uint8_t)i);
            ws2812b_load_rgba(&dev, leds);
	    tls_os_time_delay(DIM);
        }
        i -= BSTEP;
        for (; i >= 0; i -= BSTEP) {
            setcolor(col, (uint8_t)i);
            ws2812b_load_rgba(&dev, leds);
	    tls_os_time_delay(DIM);
        }
    }

    puts("Color Fading done");

    /* reset color values */
    setcolor(-1, 255);
    ws2812b_load_rgba(&dev, leds);

    while (1) {
        /* change the active color by running around the hue circle */
        col.h += 1.0;
        if (col.h > 360.0) {
            col.h = 0.0;
        }

        /* set the active LED to the active color value */
        memset(&leds[pos].color, 0, sizeof(color_rgb_t));
        pos += step;
        color_hsv2rgb(&col, &leds[pos].color);

        /* apply the values to the LED strip */
        ws2812b_load_rgba(&dev, leds);

        /* switch direction once reaching an end of the strip */
        if ((pos == (WS2812B_PARAM_LED_NUMOF -1)) || (pos == 0)) {
            step *= -1;
        }

        tls_os_time_delay(STEP);
    }

}

void UserMain(void)
{
	printf("user task\n");
	
	tls_gpio_cfg(WM_IO_PB_05, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
	tls_gpio_cfg(WM_IO_PB_11, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
	tls_gpio_cfg(WM_IO_PB_16, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
	tls_gpio_cfg(WM_IO_PB_25, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);


    tls_os_task_create(NULL, NULL,
                       demo_console_task,
                       NULL,
                       (void *)DemoTaskStk,          /* task's stack start address */
                       DEMO_TASK_SIZE * sizeof(u32), /* task's stack size, unit:byte */
                       DEMO_TASK_PRIO,
                       0);

	
//	while(1)
//	{
//	}

}

