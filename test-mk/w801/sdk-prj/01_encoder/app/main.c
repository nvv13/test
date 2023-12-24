/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: main
 *
 * Date : 2022-12-21
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"

#include "wm_cpu.h"
#include "wm_timer.h"

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_osal.h"

#include "mod1/u8g2.h"
#include "mod1/u8x8_riotos.h"
#include "encoder.h"

static u8g2_t u8g2;


#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

static const u16 i_pos_dreb_SW
    = 500; //кнопка,таймер 300 Мкс, значит будет 150 миллисекунд.
volatile static u8 i_dreb_SW = 0;     // от дребезга кнопки

static void
demo_timer_irq (u8 *arg) // здесь будет смена режима
{

  int i_enc_diff= get_encoder_diff();
  if (i_enc_diff!=0)//i_dreb_CLK != 0)
    {
      if (i_dreb_SW == 0) // защита от ддребезга контактов для кнопки
        {
          i_dreb_SW = 1;
          set_encoder_diff(0);  
        }
    }


  if(get_encoder_btn_state()==0) //Нажали
    {
      if (i_dreb_SW == 0) // защита от ддребезга контактов для кнопки
        {
          i_dreb_SW = 1;
        }
    }

  if (i_dreb_SW != 0
      && i_dreb_SW++ > i_pos_dreb_SW) //можно отсчитывать временной интервал
    {
      i_dreb_SW = 0; // от дребезга
    }

}

void
user_app1_task (void *sdata)
{
  printf ("user_app1_task start\n");

  u8 timer_id;
  struct tls_timer_cfg timer_cfg;
  timer_cfg.unit = TLS_TIMER_UNIT_US;
  timer_cfg.timeout = 300;
  timer_cfg.is_repeat = 1;
  timer_cfg.callback = (tls_timer_irq_callback)demo_timer_irq;
  timer_cfg.arg = NULL;
  timer_id = tls_timer_create (&timer_cfg);
  tls_timer_start (timer_id);
  printf ("timer start\n");

  libENCODER_t enc_pin = {
	 .ENCODER_S=WM_IO_PA_11,
	 .ENCODER_A=WM_IO_PA_12, 
	 .ENCODER_B=WM_IO_PA_13, 
   };
  bsp_encoder_init(&enc_pin);



  /* initialize to SPI */
  puts ("Initializing to I2C oled Display.");

  u8g2_Setup_sh1106_i2c_128x64_noname_f (
      &u8g2, U8G2_R0, u8x8_byte_hw_i2c_riotos, u8x8_gpio_and_delay_riotos);

  if (u8g2.u8x8.i2c_address == 255) // заменяем default на настоящий адрес
    u8g2.u8x8.i2c_address = 0x3C;

  u8x8_riotos_t d_data = {
    .pin_cs = GPIO_UNDEF,    //
    .pin_dc = GPIO_UNDEF,    //
    .pin_reset = GPIO_UNDEF, //

    .i2c_scl = WM_IO_PA_01, /* */
    .i2c_sda = WM_IO_PA_04, /* */
    .i2c_freq = 100000      /* частота i2c в герцах */

  };

  u8g2_SetUserPtr (&u8g2, &d_data);

  /* initialize the display */
  puts ("Initializing display.");

  u8g2_InitDisplay (&u8g2);
  u8g2_SetPowerSave (&u8g2, 0);



  while (1)
    { //

      tls_os_time_delay (HZ * 1);

    } //
}

void
UserMain (void)
{
  printf ("UserMain start");
  tls_sys_clk_set (CPU_CLK_240M);

  tls_os_task_create (NULL, NULL, user_app1_task, NULL,
                      (void *)UserApp1TaskStk, /* task's stack start address */
                      USER_APP1_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_APP1_TASK_PRIO, 0);
}
