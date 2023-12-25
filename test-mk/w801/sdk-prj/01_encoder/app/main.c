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
#include "wm_rtc.h"
#include "wm_timer.h"

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_osal.h"

#include "mod1/UTFT.h"
#include "mod1/u_jpeg.h"

extern uint8_t SmallFont[]; // подключаем маленький шрифт
extern uint8_t BigFont[];   // подключаем большой шрифт
extern uint8_t
    SevenSegNumFont[]; // подключаем шрифт имитирующий семисегментный индикатор
extern uint8_t SmallSymbolFont[];

#include "encoder.h"

#define USER_APP1_TASK_SIZE 2048
static OS_STK UserApp1TaskStk[USER_APP1_TASK_SIZE];
#define USER_APP1_TASK_PRIO 32

static const u16 i_pos_dreb_SW
    = 500; //кнопка,таймер 300 Мкс, значит будет 150 миллисекунд.
volatile static u8 i_dreb_SW = 0; // от дребезга кнопки

static void
demo_timer_irq (u8 *arg) // здесь будет смена режима
{

  int i_enc_diff = get_encoder_diff ();
  if (i_enc_diff != 0) // i_dreb_CLK != 0)
    {
      if (i_dreb_SW == 0) // защита от ддребезга контактов для кнопки
        {
          i_dreb_SW = 1;
          set_encoder_diff (0);
        }
    }

  if (get_encoder_btn_state () == 0) //Нажали
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
  // tls_timer_start (timer_id);
  // printf ("timer start\n");

  libENCODER_t enc_pin = {
    .ENCODER_S = WM_IO_PA_11,
    .ENCODER_A = WM_IO_PA_12,
    .ENCODER_B = WM_IO_PA_13,
  };
  bsp_encoder_init (&enc_pin);

  printf ("user_app1_task start TFT01_18SP 128x160\n");

  // подключаем библиотеку UTFT

  UTFT_UTFT (TFT01_18SP,
             (u8)NO_GPIO_PIN // WM_IO_PB_17  //RS  SDA
             ,
             (u8)NO_GPIO_PIN // WM_IO_PB_15  //WR  SCL
             ,
             WM_IO_PB_23 //(u8)NO_GPIO_PIN // WM_IO_PB_14  //CS  CS
             ,
             (u8)WM_IO_PB_21 // RST reset RES
             ,
             (u8)WM_IO_PB_22 // SER => DC !
             ,
             // 120000000
             60000000
             /* spi_freq(Герц) для 5 контактных SPI дисплеев
                (где отдельно ножка комманда/данные)
             програмируеться HW SPI на ножки (предопред)
                 wm_spi_cs_config (WM_IO_PB_14);
                 wm_spi_ck_config (WM_IO_PB_15);
                 wm_spi_di_config (WM_IO_PB_16);
                 wm_spi_do_config (WM_IO_PB_17);
             но, можно отказаться от HW SPI в пользу Soft SPI
             установив spi_freq=0
             эмуляции SPI, это удобно для разных ножек

    максимально, частота spi_freq = 20000000 (20MHz)
        но!      если spi_freq > 20000000 тогда работает spi SDIO
        частоту можно ставить от 21000000 до 120000000 герц (работает при
    240Mhz тактовой) контакты: WM_IO_PB_06 CK   -> SCL
                               WM_IO_PB_07 CMD  -> MOSI
           */
  );
  /*
  ---- ------
  W801 LCD
  ---- ------
  3.3v bl    подсветка
  3.3v vcc
  gnd  gnd
  PB21 res   RESET сброс
  PB23 cs    CS выбор чипа
  PB22 dc    комманда/данные
  PB07 sda   данные
  PB06 sck   синхросигнал
  ---- ------
  W801 LCD
  ---- ------


  */

  UTFT_InitLCD (LANDSCAPE); // инициируем дисплей
  // UTFT_InitLCD (PORTRAIT);

  UTFT_clrScr (); // стираем всю информацию с дисплея
  UTFT_setFont (SmallFont); // устанавливаем шрифт
  char msg[100];
  msg[0] = 0;

  int i_enc_diff = get_encoder_diff () + 1;
  bool btn_state = !get_encoder_btn_state ();
  while (1)
    { //

      int i_enc_curr = get_encoder_diff ();
      if (i_enc_curr != i_enc_diff)
        {
          i_enc_diff = i_enc_curr;
          if (i_enc_diff > 0)
            UTFT_setColor2 (VGA_GREEN);
          else if (i_enc_diff == 0)
            UTFT_setColor2 (VGA_WHITE);
          else
            UTFT_setColor2 (VGA_BLUE);
          sprintf (msg, " i_enc_diff=%d ", i_enc_diff);
          UTFT_print (msg, CENTER, 80, 0);
        }

      bool btn_state_curr = get_encoder_btn_state ();
      if (btn_state != btn_state_curr) //Нажали
        {
          btn_state = btn_state_curr;
          if (btn_state)
            {
              UTFT_setColor2 (VGA_WHITE);
              sprintf (msg, " button RELEASE ");
            }
          else
            {
              UTFT_setColor2 (VGA_RED);
              sprintf (msg, "  button PUSH   ");
            }
          UTFT_print (msg, CENTER, 100, 0);
        }

      tls_os_time_delay (1);

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
