/***************************************************************************** 
* 
* File Name : main.c
* 
* Description: main 
* 
* Date : 2022-06-05
*****************************************************************************/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#include "wm_regs.h"
#include "wm_rtc.h"
#include "wm_timer.h"
#include "wm_watchdog.h"
#include "wm_cpu.h"
//#include "csi_core.h"



#include "TM1637Display.h"


void UserMain(void)
{
    printf("user task\n");

    tls_sys_clk_set(CPU_CLK_240M); 

    TM1637Display(WM_IO_PB_21, WM_IO_PB_22, DEFAULT_BIT_DELAY);

    u8 u8_sec_state=0;
    setBrightness(5,1);
    uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
    setSegments(data,4,0);
    uint8_t PointData = 0x00;
    int i_cnt=0;    

    for(;;)
     {
     tls_os_time_delay(HZ);
     u8_sec_state=~u8_sec_state;

     if(u8_sec_state)
       PointData=VIEW_POINT_DATA;
       else
       PointData=0x00;

     data[0] = encodeDigit((i_cnt%10000)/1000) + PointData;
     data[1] = encodeDigit((i_cnt%1000 )/100 ) + PointData;
     data[2] = encodeDigit((i_cnt%100  )/10  ) + PointData;
     data[3] = encodeDigit( i_cnt%10         ) + PointData;
     setSegments(data,4,0);

     if(++i_cnt>9999)i_cnt=0;
     }

}

