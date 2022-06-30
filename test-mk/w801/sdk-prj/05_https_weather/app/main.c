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
//#include "wm_cpu.h"
//#include "csi_core.h"


//#include "../../../../../../w_wifi_pass.h"
//#define MY_WIFI_AP	"bred8"
//#define MY_WIFI_PASS    "123123123"
#define MY_WIFI_AP	"bred1"
#define MY_WIFI_PASS    "9115676369"

#include "w_wifi.h"
#include "w_ntp.h"
#include "w_flash_cfg.h"
#include "w_https.h"
#include "my_recognize.h"


#define  DEMO_TASK_SIZE      2048
static OS_STK 			DemoTaskStk[DEMO_TASK_SIZE];
#define  DEMO_TASK_PRIO			                32

#define  DEMO_SOCK_S_TASK_SIZE      512
static OS_STK   sock_s_task_stk[DEMO_SOCK_S_TASK_SIZE];
#define  DEMO_SOCK_S_PRIO 	                (DEMO_TASK_PRIO+1)




u32 reg_set_num(u32 reg_temp,u8 i_num) // на вход reg_temp с пинами, уже в каком то состоянии, к ним нужно добавить состояние пинов для цифры i_num (0...9)
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


#define OUT_SEC_IND 0
//#define OUT_DIG_1   1
#define OUT_DIG_2   2
#define OUT_DIG_3   3
//#define OUT_DIG_4   4

#define OUT_SIG_1   1
#define OUT_C_4     4


static u8 i_off=0;

void lcd5643printDigit(u8 i_pos,u8 i_num)
{
if(i_pos>OUT_C_4 && i_off==1)return; //выключть свет, а он уже выключен = сразу выйти
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

i_off=0; 
switch(i_pos)
{
/*
 case OUT_SEC_IND: //sec indicator, 0=9,11.7.4.2.1.10.5 1=12,8,6 - i_num=3
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
                 ;// write low 
        if(i_num==0)
           reg_temp=reg_temp  & (~(1 << 23)); // 3
           else
           reg_temp=reg_temp  | (1 << 23);    // 3
 };break;
*/

 case OUT_SIG_1: //digit 1, 0=12 1=9,8,6 - i_num=11.7.4.2.1.10.5
 {
        reg_temp=reg_temp 
         | (1 << 16)    //9
         | (1 << 17)    //8
         | (1 << 26);   //6
        reg_temp=reg_temp 
              & (~(1 << 15)); //12
        if(i_num==1) //минус
           reg_temp=reg_temp  & (~(1 << 25)); // -
           else
           reg_temp=reg_temp  | (1 << 25);    // -
 };break;

/*
 case OUT_DIG_1: //digit 1, 0=12 1=9,8,6 - i_num=11.7.4.2.1.10.5
 {
        reg_temp=reg_temp 
         | (1 << 16)    //9
         | (1 << 17)    //8
         | (1 << 26);   //6
        reg_temp=reg_temp 
              & (~(1 << 15)); //12
        reg_temp=reg_set_num(reg_temp,i_num);
 };break;
*/
 case OUT_DIG_2: //digit 2, 0=9 1=12,8,6 - i_num=11.7.4.2.1.10.5
 {
        reg_temp=reg_temp 
         | (1 << 15)    //12
         | (1 << 17)    //8
         | (1 << 26);   //6
        reg_temp=reg_temp 
              & (~(1 << 16)); //9
        reg_temp=reg_set_num(reg_temp,i_num);
 };break;
 case OUT_DIG_3: //digit 3, 0=8 1=12,9,6 - i_num=11.7.4.2.1.10.5
 {
        reg_temp=reg_temp 
         | (1 << 15)    //12
         | (1 << 16)    //9
         | (1 << 26);   //6
        reg_temp=reg_temp 
              & (~(1 << 17)); //8
        reg_temp=reg_set_num(reg_temp,i_num);
 };break;
/*
 case OUT_DIG_4: //digit 4, 0=6 1=12,9,8 - i_num=11.7.4.2.1.10.5
 {
        reg_temp=reg_temp 
         | (1 << 15)    //12
         | (1 << 16)    //9
         | (1 << 17);   //8
        reg_temp=reg_temp 
              & (~(1 << 26)); //6
        reg_temp=reg_set_num(reg_temp,i_num);
 };break;
  */
 case OUT_C_4: //digit 4, 0=6 1=12,9,8 - i_num=11.7.4.2.1.10.5
 {
        reg_temp=reg_temp 
         | (1 << 15)    //12
         | (1 << 16)    //9
         | (1 << 17);   //8
        reg_temp=reg_temp 
              & (~(1 << 26)); //6
         reg_temp=reg_temp 
         | (1 << 10) //11 A
         | (1 << 22) //2  D
         | (1 << 21) //1  E
         | (1 << 11) //10 F
        ;  // 
        reg_temp=reg_temp 
              & (~(1 << 18)) //1  B
              & (~(1 << 25)) //1  G
              & (~(1 << 24)) //1  C
         ;
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
        i_off=1; 
 };break;
}
	tls_reg_write32(HR_GPIO_DATA + offset,reg_temp );  /* write  */

        tls_reg_write32(HR_GPIO_DATA_EN + offset, reg_en); // reg_en return

	tls_os_release_critical(cpu_sr); // enable Interrupt

}


static u8 i_5643_hour =0;
static u8 i_5643_min  =0;
static u8 u8_sec_state=0;
static u16 i_out=0;

#define LCD_VAL_LG_spb_low   600
#define LCD_VAL_LG_low	     200
#define LCD_VAL_LG_middle    50
#define LCD_VAL_LG_spb_hi    10
#define LCD_VAL_LG_hi        5
u16 i_max_out=LCD_VAL_LG_spb_low;// 

static int i_5643_t_sign     =0;
static int i_5643_t_value    =0;
static int i_5643_t_mantissa =0;

static void demo_timer_irq(u8 *arg)  // здесь будет вывод на LCD
{
//        printf("timer irq hour=%d,min=%d\n",i_5643_hour,i_5643_min);

//	u8 i_HiHour=i_5643_hour/10;
//	u8 i_LoHour=i_5643_hour%10;
//	u8 i_HiMin = i_5643_min/10;
//	u8 i_LoMin = i_5643_min%10;

        //printf("timer irq hh:mm %d%d:%d%d \n",i_HiHour,i_LoHour,i_HiMin,i_LoMin);
int i_t=i_5643_t_value;
if(i_5643_t_mantissa>5)i_t++;

switch(i_out)
{
 case 0:
 {
// lcd5643printDigit(OUT_DIG_1,i_HiHour);
 if(i_5643_t_value>0)
  lcd5643printDigit(OUT_SIG_1,1);
  else
  lcd5643printDigit(OUT_SIG_1,0);

 };break;
 case 1:
 {
// lcd5643printDigit(OUT_DIG_2,i_LoHour);
 lcd5643printDigit(OUT_DIG_2,i_t/10);
 };break;
 case 2:
 {
 //lcd5643printDigit(OUT_DIG_3,i_HiMin);
 lcd5643printDigit(OUT_DIG_3,i_t%10);
 };break;
 case 3:
 {
 //lcd5643printDigit(OUT_DIG_4,i_LoMin);
 lcd5643printDigit(OUT_C_4,0);
 };break;
 case 4:
 {
 //lcd5643printDigit(OUT_SEC_IND,u8_sec_state); // on sec state
 };break;
 default:
 {
 lcd5643printDigit(5,0); // off  пока i_out будет больше 4, - выключить LCD
 };break;
}

if(i_out++>i_max_out) // от 5 ...
 {
 i_out=0;
 }

}

static u8 i_dreb=0; // от дребезга кнопки

#define DEMO_ISR_IO		WM_IO_PA_01
static void demo_gpio_isr_callback(void *context)
{

	u16 ret = tls_get_gpio_irq_status(DEMO_ISR_IO);
	//printf("\nint flag =%d\n",ret);
	if(ret)
	{
		tls_clr_gpio_irq_status(DEMO_ISR_IO);
		//if(ret == tls_gpio_read(DEMO_ISR_IO)) // button ok
                 //{
		 printf("\nbutton io =%d i_dreb=%d i_max_out=%d\n",ret,i_dreb,i_max_out);
                 if(i_dreb==0)// защита от ддребезга контактов для кнопки
                  {
                  switch(i_max_out)// градации яркости, 5-все 4 циры подряд выводит, а далее, чем больше, тем больше пропустит циклов вызова таймера для вывода
                   {
                   case LCD_VAL_LG_spb_low : i_max_out=LCD_VAL_LG_hi        ;break;
                   case LCD_VAL_LG_hi      : i_max_out=LCD_VAL_LG_spb_hi    ;break;
                   case LCD_VAL_LG_spb_hi  : i_max_out=LCD_VAL_LG_middle    ;break;
                   case LCD_VAL_LG_middle  : i_max_out=LCD_VAL_LG_low       ;break;
                   case LCD_VAL_LG_low     : i_max_out=LCD_VAL_LG_spb_low   ;break;
                   default:i_max_out=LCD_VAL_LG_spb_hi    ;break;
                   }
                  i_dreb=1;
                  }
		 //}
		//printf("\nafter int io =%d\n",ret);
	}
}



#define MEM_CELL_FROM_LIGTH_LEVEL 0

//console task use UART0 as communication port with PC
void demo_console_task(void *sdata)
{
   printf("wifi test app\n");


   u8 timer_id;
   struct tls_timer_cfg timer_cfg;

   //timer_cfg.unit = TLS_TIMER_UNIT_MS;
   //timer_cfg.timeout = 1;//4
   timer_cfg.unit = TLS_TIMER_UNIT_US; // чтобы небыло мерцания на минимальной яркости, пришлось сделать время таймера поменьше
   timer_cfg.timeout   = 25;
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

   tls_watchdog_init(60 * 1000 * 1000);//u32 usec около 1-2 минуты
   u8 i_start_reCheck=0;
   u8 u8_wifi_state=0;
   for(;;) // цикл(1) с подсоединением к wifi и запросом времени
    {
    while(u8_wifi_state==0)
	{
        printf("trying to connect wifi\n");
	if(u8_wifi_state==0 && demo_connect_net(MY_WIFI_AP,MY_WIFI_PASS)==WM_SUCCESS)
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
     
    struct tm tblock;
    tls_get_rtc(&tblock);


    if(tblock.tm_hour>=23 || tblock.tm_hour<6) // ночь, установить минимальную яркость!
     i_max_out=LCD_VAL_LG_spb_low;
     else
     {
     flash_cfg_load_u16(&i_max_out,MEM_CELL_FROM_LIGTH_LEVEL);
     printf("flash_cfg_load_u16=%d\n",i_max_out);
     if(i_max_out<LCD_VAL_LG_hi || i_max_out>LCD_VAL_LG_spb_low)
       i_max_out=LCD_VAL_LG_middle;
     i_5643_hour=i_max_out/100;
     i_5643_min =i_max_out%100;
     }

    https_demo();

    while(u8_wifi_state==1) // основной цикл(2)
     {
     tls_os_time_delay(300);
     tls_watchdog_clr();
     tls_get_rtc(&tblock);// получаем текущее время
     //printf(" sec=%d,min=%d,hour=%d,mon=%d,year=%d\n",tblock.tm_sec,tblock.tm_min,tblock.tm_hour,tblock.tm_mon+1,tblock.tm_year+1900);
     u8_sec_state=~u8_sec_state;

     if(i_dreb==1) //нажали кнопку, сохраним значение
      {
      i_5643_hour=i_max_out/100;
      i_5643_min =i_max_out%100;
      printf("flash_cfg_store_u16=%d\n",i_max_out);
      flash_cfg_store_u16(i_max_out, MEM_CELL_FROM_LIGTH_LEVEL);
      i_dreb=0;// защита от ддребезга контактов для кнопки
      }
      else
      {
      i_5643_hour=tblock.tm_hour;
      i_5643_min =tblock.tm_min;
      }


     i_5643_hour=tblock.tm_hour;
     i_5643_min =tblock.tm_min;

     i_5643_t_sign     =my_recognize_ret_cur_temperature_sign();
     i_5643_t_value    =my_recognize_ret_cur_temperature();
     i_5643_t_mantissa =my_recognize_ret_cur_temperature_mantissa();

     //if((tblock.tm_min==0 || tblock.tm_min==10 || tblock.tm_min==20 || tblock.tm_min==30 || tblock.tm_min==40 || tblock.tm_min==50 ||
     //    tblock.tm_min==5 || tblock.tm_min==15 || tblock.tm_min==25 || tblock.tm_min==35 || tblock.tm_min==45 || tblock.tm_min==55
     //   ) && tblock.tm_sec==0) // запросим снова ntp, - синхр время раз в сутки
     if((tblock.tm_hour==3 || i_start_reCheck<2 /* было, мигнул свет и... , вообщим добавим еще разок другой */ ) && tblock.tm_min==0 && tblock.tm_sec==0) // запросим снова ntp, - синхр время раз в сутки
            {
	    i_start_reCheck++;
            u8_wifi_state=0; // переход на цикл(1)
     	    //tls_sys_reset(); так то, это не надо, вроде все стабильно работает
            }
 
     if(tblock.tm_hour==23 && tblock.tm_min==0 && tblock.tm_sec==0 && i_max_out!=LCD_VAL_LG_spb_low) // ночь, установить минимальную яркость!
            {
            i_max_out=LCD_VAL_LG_spb_low;
            }

     if(tblock.tm_hour==6 && tblock.tm_min==0 && tblock.tm_sec==0 && i_max_out==LCD_VAL_LG_spb_low) // утро, восстановить яркость!
            {
            flash_cfg_load_u16(&i_max_out,MEM_CELL_FROM_LIGTH_LEVEL);
            if(i_max_out<LCD_VAL_LG_hi || i_max_out>LCD_VAL_LG_spb_low)
              i_max_out=LCD_VAL_LG_middle;
            }
     if((tblock.tm_min==0 || (tblock.tm_min>0 && tblock.tm_min%5==0) )&& tblock.tm_sec==0 
        && my_recognize_ret_cur_temperature()!=MY_RECOGNIZE_NO_VALUE)
      {
      printf("cur_temperature=%d,%d\n"
            ,my_recognize_ret_cur_temperature_sign() * my_recognize_ret_cur_temperature()
            ,my_recognize_ret_cur_temperature_mantissa());
      printf("    date %d.%02d.%02d %02d:%02d:%02d\n"
           ,tblock.tm_year+1900
           ,tblock.tm_mon+1
           ,tblock.tm_mday
           ,tblock.tm_hour
           ,tblock.tm_min
           ,tblock.tm_sec
         );
      tls_os_time_delay(1000 - 300);
      }

     //
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

    tls_os_task_create(NULL, NULL,
                       sock_s_task,
                       NULL,
                       (void *)sock_s_task_stk,          /* task's stack start address */
                       DEMO_SOCK_S_TASK_SIZE * sizeof(u32), /* task's stack size, unit:byte */
                       DEMO_SOCK_S_PRIO,
                       0);

//    tls_os_time_delay(1000 * 10);
//    tls_os_disp_task_stat_info();
//	while(1)
//	{
//	}

}

