/***************************************************************************** 
* 
* File Name : main.c
* 
* Description: main 
* 
* Copyright (c) 2014 Winner Micro Electronic Design Co., Ltd. 
* All rights reserved. 
* 
* Author : dave
* 
* Date : 2014-6-14
*****************************************************************************/ 

#include "wm_gpio.h"

void UserMain(void)
{
	printf("user task\n");
	
	tls_gpio_cfg(WM_IO_PB_05, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
	tls_gpio_cfg(WM_IO_PB_11, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
	
	u8 u8_led_state=0;
	while(1)
	{
	    tls_gpio_write(WM_IO_PB_05, u8_led_state);	
	    tls_gpio_write(WM_IO_PB_11, u8_led_state);
	    tls_os_time_delay(200);
	    u8_led_state=~u8_led_state;
	}

}

