/*
  источник
  https://github.com/openwch/ch32v003.git
*/

#include <stdlib.h>

#include <cstring>

#include "debug.h"

#include "CShell.hpp"

#include "lcd5643.hpp"

static CShell *PoCShell = NULL;
static u8 b_intensity = 100;

#define CFG_intensity_BYTE 1

int
clock_int_get (int argc, char **argv)
{
  u8 u8_value = b_intensity;
  PoCShell->oUsart->SendPSZstring ("The current intensity is: ");
  PoCShell->oUsart->SendIntToStr (b_intensity);
  PoCShell->oUsart->SendPSZstring ("\r\n");
  return 0;
}

int
clock_int_set (int argc, char **argv)
{
  if (argc != 2)
    {
      PoCShell->oUsart->SendPSZstring ("usage: ");
      PoCShell->oUsart->SendPSZstring (argv[0]);
      PoCShell->oUsart->SendPSZstring ("0-254\r\n");
      return 1;
    }

  u8 u8_value = atoi (argv[1]);
  b_intensity = u8_value;
  PoCShell->oUsart->SendPSZstring ("success: intensity set to ");
  PoCShell->oUsart->SendIntToStr (b_intensity);
  PoCShell->oUsart->SendPSZstring ("\r\n");
  return 0;
}

static const shell_command_t shell_commands[] = {
  //{ "init", "Setup a particular SPI configuration", cmd_init },
  { "iget", "get current intensity", clock_int_get },
  { "iset", "set intensity 0-254", clock_int_set },
  { NULL, NULL, NULL }
};

static volatile bool l_flag = false;

static u8 i_5643_hour = 0;
static u8 i_5643_min = 0;
static u8 u8_sec_state = 0;
void update_disp (void); // здесь будет вывод на LCD

int
main (void)
{
  NVIC_PriorityGroupConfig (NVIC_PriorityGroup_1);
  SystemCoreClockUpdate ();
  Delay_Init ();
  // USART_Printf_Init (115000);
  CShell oCShell = CShell (shell_commands);
  PoCShell = &oCShell;
  PoCShell->oUsart->SendPSZstring ("SystemClk1:");
  PoCShell->oUsart->SendIntToStr (SystemCoreClock);
  PoCShell->oUsart->SendPSZstring ("\r\n");
  PoCShell->oUsart->SendPSZstring ("enter help for usage\r\n");

  lcd5643_init_pin ();
  PoCShell->oUsart->SendPSZstring ("start\r\n");
  // NVIC_EnableIRQ (SysTicK_IRQn);
  // SysTick->SR &= ~(1 << 0);
  // SysTick->CMP = SystemCoreClock / 10; //
  // SysTick->CNT = 0;
  // SysTick->CTLR = 0xF;

  int i_cnt_disp = 0;
  while (1)
    {
      oCShell.Idle ();
      //Delay_Ms (10);
      l_flag = true;
      // ds3231_cmd_get (0, NULL);
      if (l_flag)
        {
          // NVIC_DisableIRQ (SysTicK_IRQn);
          //__disable_irq ();
          l_flag = false;
          //__enable_irq ();
          // NVIC_EnableIRQ (SysTicK_IRQn);
          if (i_cnt_disp++ > 500)
            {
              // PoCShell->oUsart->SendPSZstring ("i_cnt_disp++ > 1000\r\n");
              i_cnt_disp = 0;
              i_5643_min++;
              if (i_5643_min > 59)
                {
                  i_5643_hour++;
                  i_5643_min = 0;
                }
              if (i_5643_hour > 23)
                {
                  i_5643_hour = 0;
                };
              u8_sec_state = ~u8_sec_state;
              PoCShell->oUsart->SendPSZstring ("i_5643_min:");
              PoCShell->oUsart->SendIntToStr (i_5643_min);
              PoCShell->oUsart->SendPSZstring ("\r\n");
            }
              update_disp ();
          //__enable_irq ();
          // NVIC_EnableIRQ (SysTicK_IRQn);
        }
    }
}

// void SysTick_Handler (void) __attribute__ ((interrupt
// ("WCH-Interrupt-fast")));
// extern "C" void SysTick_Handler (void) __attribute__ ((interrupt
// ("machine"))); extern "C" void SysTick_Handler (void)
//{
//  l_flag = true;
//  SysTick->SR = 0;
//}

static u16 i_out = 0;

#define LCD_VAL_LG_spb_low 600
#define LCD_VAL_LG_low 200
#define LCD_VAL_LG_middle 50
#define LCD_VAL_LG_spb_hi 10
#define LCD_VAL_LG_hi 5
u16 i_max_out = LCD_VAL_LG_middle; //

static int i_5643_t_sign = 0;
static int i_5643_t_value = 88;
static int i_5643_t_mantissa = 0;

static u8 iMode = MODE_CLOCK;

void
update_disp (void) // здесь будет вывод на LCD
{
  //        printf("timer irq hour=%d,min=%d\n",i_5643_hour,i_5643_min);
  u8 i_HiHour;
  u8 i_LoHour;
  u8 i_HiMin;
  u8 i_LoMin;

  if (iMode == MODE_CLOCK)
    {
      i_HiHour = i_5643_hour / 10;
      i_LoHour = i_5643_hour % 10;
      i_HiMin = i_5643_min / 10;
      i_LoMin = i_5643_min % 10;
    }

  // printf("timer irq hh:mm %d%d:%d%d \n",i_HiHour,i_LoHour,i_HiMin,i_LoMin);
  int i_t = i_5643_t_value;
  if (i_5643_t_mantissa > 5)
    i_t++;

  switch (i_out)
    {
    case 0:
      {
        if (iMode == MODE_CLOCK)
          lcd5643printDigit (OUT_DIG_1, i_HiHour, iMode);
        else
          {
            if (i_5643_t_sign > 0)
              lcd5643printDigit (OUT_SIG_1, 1, iMode); // "+"
            else
              lcd5643printDigit (OUT_SIG_1, 0, iMode); // "-"
          }
      };
      break;
    case 1:
      {
        if (iMode == MODE_CLOCK)
          lcd5643printDigit (OUT_DIG_2, i_LoHour, iMode);
        else
          lcd5643printDigit (OUT_DIG_2, i_t / 10, iMode);
      };
      break;
    case 2:
      {
        if (iMode == MODE_CLOCK)
          lcd5643printDigit (OUT_DIG_3, i_HiMin, iMode);
        else
          lcd5643printDigit (OUT_DIG_3, i_t % 10, iMode);
      };
      break;
    case 3:
      {
        if (iMode == MODE_CLOCK)
          lcd5643printDigit (OUT_DIG_4, i_LoMin, iMode);
        else
          lcd5643printDigit (OUT_C_4, 0, iMode);
      };
      break;
    case 4:
      {
        if (iMode == MODE_CLOCK)
          lcd5643printDigit (OUT_SEC_IND, u8_sec_state, iMode); // on sec state
      };
      break;
    default:
      {
        lcd5643printDigit (
            5, 0, iMode); // off  пока i_out будет больше 4, - выключить LCD
      };
      break;
    }

  if (i_out++ > i_max_out) // от 5 ...
    {
      i_out = 0;
    }
}
