/***************************************************************************** 
* 
* File Name : main.c
* 
* Description: main 
* 
* Date : 2022-05-19
*****************************************************************************/ 

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
#include "wm_rtc.h"

#include "w_wifi.h"
#include "w_ntp.h"


#define    DEMO_TASK_SIZE      2048
static OS_STK 			DemoTaskStk[DEMO_TASK_SIZE];
#define  DEMO_TASK_PRIO			                32


//console task use UART0 as communication port with PC
void demo_console_task(void *sdata)
{

    //puts("WIFI Test App");
    printf("wifi test app\n");

    u8 u8_wifi_state=0;
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
 
    u8 u8_ntp_state=0;
    while(u8_ntp_state==0)
	{
        printf("trying to get ntp\n");
	if(u8_ntp_state==0 && ntp_demo()==WM_SUCCESS)
	    u8_ntp_state=1;
   	    else
            {
	    tls_os_time_delay(5000);
            }
	}

    u8 u8_led_state=0;
  

    for(;;)
    {

	{
	    tls_gpio_write(WM_IO_PB_05, u8_led_state);	
	    tls_gpio_write(WM_IO_PB_11, u8_led_state);
	    tls_gpio_write(WM_IO_PB_16, ~u8_led_state);
	    tls_gpio_write(WM_IO_PB_25, ~u8_led_state);
	    tls_os_time_delay(1000);
	    u8_led_state=~u8_led_state;


	    struct tm tblock;
 	    tls_get_rtc(&tblock);
            printf(" sec=%d,min=%d,hour=%d,mon=%d,year=%d\n",tblock.tm_sec,tblock.tm_min,tblock.tm_hour,tblock.tm_mon+1,tblock.tm_year+1900);


	}
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

