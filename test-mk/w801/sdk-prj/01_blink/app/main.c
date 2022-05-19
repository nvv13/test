/***************************************************************************** 
* 
* File Name : main.c
* 
* Description: main 
* 
* Date : 2022-05-19
*****************************************************************************/ 

#include <stdio.h>
#include "wm_type_def.h"
#include "wm_cpu.h"
#include "wm_gpio.h"

void UserMain(void)
{
	printf("user task\n");


	
	tls_gpio_cfg(WM_IO_PB_01, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
	tls_gpio_cfg(WM_IO_PB_11, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
	
//static int pwm_demo_allsyc_mode(u8 channel,u32 freq, u8 duty, u8 num)

	tls_sys_clk_set(CPU_CLK_240M);

	tls_sys_clk sysclk;
	tls_sys_clk_get(&sysclk);
//    pwm_param.clkdiv = sysclk.apbclk*UNIT_MHZ/256/freq;

	printf("  sysclk.apbclk %d\n",sysclk.apbclk);
	printf("  sysclk.apbclk*UNIT_MHZ %d\n",sysclk.apbclk*UNIT_MHZ);

	printf("  sysclk.cpuclk %d\n",sysclk.cpuclk);
	printf("  sysclk.cpuclk*UNIT_MHZ %d\n",sysclk.cpuclk*UNIT_MHZ);


	u8 u8_led_state=0;
	while(1)
	{
	    tls_gpio_write(WM_IO_PB_01, u8_led_state);	
//	    tls_gpio_write(WM_IO_PB_11, u8_led_state);
//	    tls_os_time_delay(200);
	    u8_led_state=~u8_led_state;
	}

}

