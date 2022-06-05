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
#include "wm_regs.h"
#include "wm_rtc.h"
#include "wm_timer.h"
#include "wm_watchdog.h"

//#include "wm_cpu.h"
//#include "csi_core.h"

#include "w_wifi.h"
#include "w_ntp.h"


#define    DEMO_TASK_SIZE      2048
static OS_STK 			DemoTaskStk[DEMO_TASK_SIZE];
#define  DEMO_TASK_PRIO			                32

u32 reg_set_num(u32 reg_temp,u8 i_num)
{
/*
PB_21 1
PB_22 2
PB_23 3
PB_24 4
PB_25 5
PB_26 6
PB_18 7
PB_17 8
PB_16 9
PB_11 10
PB_10 11
PB_15 12
*/
//i_num=11.7.4.2.1.10.5
switch(i_num)
{
 case 0: // 0=5  1=11 7 4 2 1 10
 {
 reg_temp=reg_temp 
         | (1 << 10) //11 A
         | (1 << 18) //7  B
         | (1 << 24) //4  C
         | (1 << 22) //2  D
         | (1 << 21) //1  E
         | (1 << 11) //10 F
         ;
 reg_temp=reg_temp 
              & (~(1 << 25)) //5 G
             ;/* write low */
 };break;
 case 1: // 
 {
 reg_temp=reg_temp 
         | (1 << 18) //7  B
         | (1 << 24) //4  C
        ;  // 
 reg_temp=reg_temp 
              & (~(1 << 10)) //11 A
              & (~(1 << 22)) //2  D
              & (~(1 << 21)) //1  E
              & (~(1 << 11)) //10 F
              & (~(1 << 25)) //5 G
             ;/* write low */
 };break;
 case 2: // 
 {
 reg_temp=reg_temp 
         | (1 << 10) //11 A
         | (1 << 18) //7  B
         | (1 << 22) //2  D
         | (1 << 21) //1  E
         | (1 << 25) //5 G
        ;  // 
 reg_temp=reg_temp 
              & (~(1 << 11)) //10 F
              & (~(1 << 24)) //4  C
             ;/* write low */
 };break;
 case 3: // 
 {
 reg_temp=reg_temp 
         | (1 << 10) //11 A
         | (1 << 18) //7  B
         | (1 << 22) //2  D
         | (1 << 24) //4  C
         | (1 << 25) //5 G
        ;  // 
 reg_temp=reg_temp 
              & (~(1 << 11)) //10 F
              & (~(1 << 21)) //1  E
             ;/* write low */
 };break;
 case 4: // 
 {
 reg_temp=reg_temp 
         | (1 << 18) //7  B
         | (1 << 24) //4  C
         | (1 << 11) //10 F
         | (1 << 25) //5 G
        ;  // 
 reg_temp=reg_temp 
              & (~(1 << 10)) //11 A
              & (~(1 << 22)) //2  D
              & (~(1 << 21)) //1  E
             ;/* write low */
 };break;
 case 5: // 
 {
 reg_temp=reg_temp 
         | (1 << 10) //11 A
         | (1 << 24) //4  C
         | (1 << 22) //2  D
         | (1 << 11) //10 F
         | (1 << 25) //5 G
        ;  // 
 reg_temp=reg_temp 
              & (~(1 << 18)) //7  B
              & (~(1 << 21)) //1  E
             ;/* write low */
 };break;
 case 6: // 
 {
 reg_temp=reg_temp 
         | (1 << 10) //11 A
         | (1 << 24) //4  C
         | (1 << 22) //2  D
         | (1 << 21) //1  E
         | (1 << 11) //10 F
         | (1 << 25) //5 G
        ;  // 
 reg_temp=reg_temp 
              & (~(1 << 18)) //7  B
             ;/* write low */
 };break;
 case 7: // 
 {
 reg_temp=reg_temp 
         | (1 << 10) //11 A
         | (1 << 18) //7  B
         | (1 << 24) //4  C
        ;  // 
 reg_temp=reg_temp 
              & (~(1 << 22)) //2  D
              & (~(1 << 21)) //1  E
              & (~(1 << 11)) //10 F
              & (~(1 << 25)) //5 G
             ;/* write low */
 };break;
 case 8: // 
 {
 reg_temp=reg_temp 
         | (1 << 10) //11 A
         | (1 << 18) //7  B
         | (1 << 24) //4  C
         | (1 << 22) //2  D
         | (1 << 21) //1  E
         | (1 << 11) //10 F
         | (1 << 25) //5 G
        ;  // 
 };break;
 case 9: // 
 {
 reg_temp=reg_temp 
         | (1 << 10) //11 A
         | (1 << 18) //7  B
         | (1 << 24) //4  C
         | (1 << 22) //2  D
         | (1 << 11) //10 F
         | (1 << 25) //5 G
        ;  // 
 reg_temp=reg_temp 
              & (~(1 << 21)) //1  E
        ;  // 
 };break;
}
return reg_temp;
}

void lcd5643printDigit(u8 i_pos,u8 i_num)
{
/*
LCD display 5643AS-1
     12 pin

connect to
W801  5643AS-1 
GPIO  PIN
PB_21 1
PB_22 2
PB_23 3
PB_24 4
PB_25 5
PB_26 6
PB_18 7
PB_17 8
PB_16 9
PB_11 10
PB_10 11
PB_15 12

*/

	u32 cpu_sr = cpu_sr = tls_os_set_critical();  // disable Interrupt !!!

        u16 offset = TLS_IO_AB_OFFSET;
	u32 reg;
	u32 reg_en;

	reg_en = tls_reg_read32(HR_GPIO_DATA_EN + offset);
	tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en 
         | (1 << 21)
         | (1 << 22)
         | (1 << 23)
         | (1 << 24)
         | (1 << 25)
         | (1 << 26)
         | (1 << 18)
         | (1 << 17)
         | (1 << 16)
         | (1 << 11)
         | (1 << 10)
         | (1 << 15)
           ); // enabled control reg from need pin

	reg = tls_reg_read32(HR_GPIO_DATA + offset); // load all pins from port

	u32 reg_temp=reg;

switch(i_pos)
{
 case 0: //sec indicator, 0=9,11.7.4.2.1.10.5 1=12,8,6 - i_num=3
 {
        reg_temp=reg_temp 
         | (1 << 15)   // 12
         | (1 << 17)   // 8
         | (1 << 26);  // 6

        reg_temp=reg_temp 
              & (~(1 << 16))
              & (~(1 << 10))
              & (~(1 << 18))
              & (~(1 << 24))
              & (~(1 << 22))
              & (~(1 << 21))
              & (~(1 << 11))
              & (~(1 << 25))
                 ;/* write low */
        if(i_num==0)
           reg_temp=reg_temp  & (~(1 << 23)); // 3
           else
           reg_temp=reg_temp  | (1 << 23);    // 3
 };break;
 case 1: //digit 1, 0=12 1=9,8,6 - i_num=11.7.4.2.1.10.5
 {
        reg_temp=reg_temp 
         | (1 << 16)    //9
         | (1 << 17)    //8
         | (1 << 26);   //6
        reg_temp=reg_temp 
              & (~(1 << 15)); //12
        reg_temp=reg_set_num(reg_temp,i_num);
 };break;
 case 2: //digit 2, 0=9 1=12,8,6 - i_num=11.7.4.2.1.10.5
 {
        reg_temp=reg_temp 
         | (1 << 15)    //12
         | (1 << 17)    //8
         | (1 << 26);   //6
        reg_temp=reg_temp 
              & (~(1 << 16)); //9
        reg_temp=reg_set_num(reg_temp,i_num);
 };break;
 case 3: //digit 3, 0=8 1=12,9,6 - i_num=11.7.4.2.1.10.5
 {
        reg_temp=reg_temp 
         | (1 << 15)    //12
         | (1 << 16)    //9
         | (1 << 26);   //6
        reg_temp=reg_temp 
              & (~(1 << 17)); //8
        reg_temp=reg_set_num(reg_temp,i_num);
 };break;
 case 4: //digit 4, 0=6 1=12,9,8 - i_num=11.7.4.2.1.10.5
 {
        reg_temp=reg_temp 
         | (1 << 15)    //12
         | (1 << 16)    //9
         | (1 << 17);   //8
        reg_temp=reg_temp 
              & (~(1 << 26)); //6
        reg_temp=reg_set_num(reg_temp,i_num);
 };break;
 default: // off ligth
 {
        reg_temp=reg_temp 
              & (~(1 << 15))
              & (~(1 << 17))
              & (~(1 << 26))
              & (~(1 << 16))
              & (~(1 << 10))
              & (~(1 << 18))
              & (~(1 << 24))
              & (~(1 << 22))
              & (~(1 << 21))
              & (~(1 << 11))
              & (~(1 << 25))
              & (~(1 << 23))
                 ;/* write low */
 };break;
}
	tls_reg_write32(HR_GPIO_DATA + offset,reg_temp );  /* write  */

        tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en); // reg_en return

	tls_os_release_critical(cpu_sr); // enable Interrupt

}


static u8 i_5643_hour =0;
static u8 i_5643_min  =0;
static u8 u8_sec_state=0;
static u8 i_out=0;
static u8 i_max_out=200;

static void demo_timer_irq(u8 *arg)
{
//        printf("timer irq hour=%d,min=%d\n",i_5643_hour,i_5643_min);

	u8 i_HiHour=i_5643_hour/10;
	u8 i_LoHour=i_5643_hour%10;
	u8 i_HiMin=i_5643_min/10;
	u8 i_LoMin=i_5643_min%10;
        //printf("timer irq hh:mm %d%d:%d%d \n",i_HiHour,i_LoHour,i_HiMin,i_LoMin);

switch(i_out)
{
 case 0:
 {
 lcd5643printDigit(1,i_HiHour);
 };break;
 case 1:
 {
 lcd5643printDigit(2,i_LoHour);
 };break;
 case 2:
 {
 lcd5643printDigit(3,i_HiMin);
 };break;
 case 3:
 {
 lcd5643printDigit(4,i_LoMin);
 };break;
 case 4:
 {
 lcd5643printDigit(0,u8_sec_state); // on sec state
 };break;
 default:
 {
 lcd5643printDigit(5,0); // off 
 };break;
}

if(i_out++>i_max_out) // от 5 ...
 {
 i_out=0;
 }

}

static u8 i_dreb=0;

#define DEMO_ISR_IO		WM_IO_PA_01
static void demo_gpio_isr_callback(void *context)
{

	u16 ret = tls_get_gpio_irq_status(DEMO_ISR_IO);
	//printf("\nint flag =%d\n",ret);
	if(ret)
	{
		tls_clr_gpio_irq_status(DEMO_ISR_IO);
		if(ret == tls_gpio_read(DEMO_ISR_IO)) // button ok
                 {
		 printf("\nbutton io =%d\n",ret);
                 if(i_dreb==0)
                  {
                  switch(i_max_out)
                   {
                   case 200 : i_max_out=5;break;
                   //case 100 : i_max_out=200;break;
                   case 50  : i_max_out=200;break;
                   case 5   : i_max_out=50;break;
                   }
                  i_dreb=1;
                  }
		 }
		//printf("\nafter int io =%d\n",ret);
	}
}

//console task use UART0 as communication port with PC
void demo_console_task(void *sdata)
{
    //puts("WIFI Test App");
    printf("wifi test app\n");

    u8 timer_id;
    struct tls_timer_cfg timer_cfg;

    //timer_cfg.unit = TLS_TIMER_UNIT_MS;
    //timer_cfg.timeout = 1;//4
    timer_cfg.unit = TLS_TIMER_UNIT_US;
    timer_cfg.timeout = 100;
    timer_cfg.is_repeat = 1;
    timer_cfg.callback = (tls_timer_irq_callback)demo_timer_irq;
    timer_cfg.arg = NULL;
    timer_id = tls_timer_create(&timer_cfg);
    tls_timer_start(timer_id);
    printf("timer start\n");	

	u16 gpio_pin;
	gpio_pin = DEMO_ISR_IO;
	//
	tls_gpio_cfg(gpio_pin, WM_GPIO_DIR_INPUT, WM_GPIO_ATTR_FLOATING);
	tls_gpio_isr_register(gpio_pin, demo_gpio_isr_callback, NULL);
	tls_gpio_irq_enable(gpio_pin, WM_GPIO_IRQ_TRIG_RISING_EDGE);
	printf("\nbutton gpio %d rising isr\n",gpio_pin);


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

    for(;;)
    {

    tls_gpio_write(WM_IO_PB_05, u8_sec_state);	
    tls_os_time_delay(500);

    struct tm tblock;
    tls_get_rtc(&tblock);
    //printf(" sec=%d,min=%d,hour=%d,mon=%d,year=%d\n",tblock.tm_sec,tblock.tm_min,tblock.tm_hour,tblock.tm_mon+1,tblock.tm_year+1900);

    i_5643_hour=tblock.tm_hour;
    i_5643_min =tblock.tm_min;
    //i_5643_hour=tblock.tm_min;
    //i_5643_min =tblock.tm_sec;
    u8_sec_state=~u8_sec_state;

    i_dreb=0;

    if(tblock.tm_hour==3 && tblock.tm_min==0 && tblock.tm_sec==0) // запросим снова ntp, синхр время
    	    tls_sys_reset();

    }
}

void UserMain(void)
{
	printf("user task\n");
	
	tls_gpio_cfg(WM_IO_PB_05, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);

	tls_gpio_cfg(WM_IO_PB_21, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW);// 1
	tls_gpio_cfg(WM_IO_PB_22, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW);// 2
	tls_gpio_cfg(WM_IO_PB_23, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW);// 3
	tls_gpio_cfg(WM_IO_PB_24, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW);// 4
	tls_gpio_cfg(WM_IO_PB_25, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW);// 5
	tls_gpio_cfg(WM_IO_PB_26, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW);// 6
	tls_gpio_cfg(WM_IO_PB_18, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW);// 7
	tls_gpio_cfg(WM_IO_PB_17, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW);// 8
	tls_gpio_cfg(WM_IO_PB_16, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW);// 9
	tls_gpio_cfg(WM_IO_PB_11, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW);// 10
	tls_gpio_cfg(WM_IO_PB_10, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW);// 11
	tls_gpio_cfg(WM_IO_PB_15, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_PULLLOW);// 12


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

