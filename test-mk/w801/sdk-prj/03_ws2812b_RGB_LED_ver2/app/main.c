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
#include "wm_type_def.h"
#include "wm_cpu.h"
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
//#include "wm_timer.h"
#include "wm_rtc.h"
#include "wm_watchdog.h"

#include "ws2812b.h"
#include "ws2812b_params.h"

#include "w_wifi.h"
#include "w_flash_cfg.h"
#include "decode_cmd.h"


#define  DEMO_TASK_SIZE      1024
static OS_STK 			DemoTaskStk[DEMO_TASK_SIZE];
#define  DEMO_TASK_PRIO			           60

#define  DEMO_SOCK_S_TASK_SIZE      1024
static OS_STK   sock_s_task_stk[DEMO_SOCK_S_TASK_SIZE];
#define  DEMO_SOCK_S_PRIO 	                (32)


u16 i_light=10;
u16 i_swith=2;
struct tm t_last_start_main_task;


#define US_PER_MS	1

/**
 * @brief   Switch to the next LED every 10ms
 */
#define STEP        (20 * US_PER_MS)

/**
 * @brief   Interval for dimming colors
 */
#define DIM         (20 * US_PER_MS)

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

void ef_1(void)
{
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

    /* reset color values */
    setcolor(-1, 255);
    ws2812b_load_rgba(&dev, leds);

    puts("Color Fading done");
}


int pos = 0;
int step = 1;
color_hsv_t col = { 0.0, 1.0, 1.0 };

void ef_2(void)
{
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

        puts("ef_2 done");
}

u8 u8_wait_start_ota_upgrade;


void demo_console_task(void *sdata)
{
   u8_wait_start_ota_upgrade=0;
   tls_get_rtc(&t_last_start_main_task);

//    ws2812b_params_t param;
//    param.led_numof = WS2812B_PARAM_LED_NUMOF;
    //param.data_pin  = WM_IO_PB_01;
//    param.data_pin  = WM_IO_PA_10;



tls_sys_clk_set(CPU_CLK_240M); // нам мужно 240MHz, под это всё подогнано

    /* initialize all LED color values to black (off) */
    memset(leds, 0, sizeof(color_rgba_t) * WS2812B_PARAM_LED_NUMOF);

    ws2812b_params_t param;
    param.led_numof = WS2812B_PARAM_LED_NUMOF;
    param.data_pin  = WM_IO_PB_01;

    ws2812b_init(&dev, &param );

    puts("Initialization done.");

    /* reset color values */
    setcolor(-1, 255);
    ws2812b_load_rgba(&dev, leds);

tls_watchdog_init(60 * 1000 * 1000);//u32 usec около 1 минуты

u8 u8_wifi_state=0;
for(;;) // цикл(1) с подсоединением к wifi и запросом времени
 {
    while(u8_wifi_state==0)
	{
        printf("trying to connect wifi\n");
	if(u8_wifi_state==0 && demo_connect_net("bred1","9115676369")==WM_SUCCESS)
	    u8_wifi_state=1;
   	    else
            {
	    tls_os_time_delay(5000);
            }
	}

    puts("WS2812B Test App");




    while (u8_wifi_state) {

	switch(i_swith)
	{
	case 1:
	 {
	 ef_1();
	 };break;
	case 2:
	 {
	 ef_2();
	 };break;
	case 3:
	 {
   	 /* reset color values */
         puts("reset color value");
    	 setcolor(-1, 255);
    	 ws2812b_load_rgba(&dev, leds);
         tls_os_time_delay(STEP);
	 };break;
	default:
	 {
         tls_os_time_delay(STEP);
 	 };break;
	}

        tls_watchdog_clr();//сбросить


     if(u8_wait_start_ota_upgrade)
       {
       u8_wait_start_ota_upgrade=0;
       printf("OTA upgrade start, try = " OTA_PATH_FILE "\n" );
       //tls_timer_stop(timer_id);
       tls_watchdog_clr();
       t_http_fwup(OTA_PATH_FILE);
       printf("OTA upgrade stop, error\n" );//если в это место попало, значит какая-то ошибка случилась и прошивка не скачалась
       //tls_timer_start(timer_id);
       }

    }

 }

}


void sock_s_task(void *sdata)
{

    while(1)
    {
     int i_port=5555;
     printf("create_socket_server  i_port=%d\n", i_port);
     create_socket_server(i_port);
    }

}

void UserMain(void)
{
	printf("user task\n");
	
	//tls_sys_clk sysclk;
	//tls_sys_clk_get(&sysclk);
	//printf("  sysclk.apbclk %d\n",sysclk.apbclk);
	//printf("  sysclk.cpuclk %d\n",sysclk.cpuclk);

    tls_os_task_create(NULL, NULL,
                       demo_console_task,
                       NULL,
                       (void *)DemoTaskStk,          /* task's stack start address */
                       DEMO_TASK_SIZE * sizeof(u32), /* task's stack size, unit:byte */
                       DEMO_TASK_PRIO,
                       0);

    tls_os_task_create(NULL, NULL,
                       sock_s_task,
                       NULL,
                       (void *)sock_s_task_stk,          /* task's stack start address */
                       DEMO_SOCK_S_TASK_SIZE * sizeof(u32), /* task's stack size, unit:byte */
                       DEMO_SOCK_S_PRIO,
                       0);
	
//	while(1)
//	{
//	}

}

