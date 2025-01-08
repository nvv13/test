#include "debug.h"

#include "lcd5643.h"

#define OUT_SEC_IND 0
#define OUT_SIG_1 1
#define OUT_DIG_1 1
#define OUT_DIG_2 2
#define OUT_DIG_3 3
#define OUT_DIG_4 4
#define OUT_C_4 4

#define DGOUP_E GPIOA
#define DPORT_E GPIO_Pin_1

#define DGOUP_D GPIOD
#define DPORT_D GPIO_Pin_4

#define DGOUP_DP GPIOD
#define DPORT_DP GPIO_Pin_3

#define DGOUP_C GPIOD
#define DPORT_C GPIO_Pin_2

#define DGOUP_G GPIOA
#define DPORT_G GPIO_Pin_2

#define DGOUP_Dig4 GPIOD
#define DPORT_Dig4 GPIO_Pin_0

#define DGOUP_B GPIOC
#define DPORT_B GPIO_Pin_7

#define DGOUP_Dig3 GPIOC
#define DPORT_Dig3 GPIO_Pin_6

#define DGOUP_Dig2 GPIOC
#define DPORT_Dig2 GPIO_Pin_5

#define DGOUP_F GPIOC
#define DPORT_F GPIO_Pin_4

#define DGOUP_A GPIOC
#define DPORT_A GPIO_Pin_3

#define DGOUP_Dig1 GPIOC
#define DPORT_Dig1 GPIO_Pin_0

/*
PA1  1  E
PD4  2  D
PD3  3  DP
PD2  4  C
PA2  5  G
PD0  6  Dig4
PC7  7  B
PC6  8  Dig3
PC5  9  Dig2
PC4  10 F
PC3  11 A
PC0  12 Dig1
*/

static BitAction D_ON = Bit_SET;
static BitAction D_OFF = Bit_RESET;

void
lcd5643_init_pin (u8 i_common_mode)
{
  i_5643_out = 0;
  i_5643_max_out = LCD_VAL_LG_hi; 
  if (i_common_mode == MODE_COMMON_CATHODE)
    {
      D_ON = Bit_SET;
      D_OFF = Bit_RESET;
    }
  else
    {
      D_OFF = Bit_SET;
      D_ON = Bit_RESET;
    }

  RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOD, ENABLE);

  GPIOSpeed_TypeDef GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitTypeDef GPIO_InitStructure = { 0 };

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_4 | GPIO_Pin_3
                                | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed;
  GPIO_Init (GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5
                                | GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_0;
  GPIO_Init (GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_Init (GPIOA, &GPIO_InitStructure);
}

static void
reg_set_num (u8 i_num) //
{

  switch (i_num)
    {
    case 0:
      {
        GPIO_WriteBit (DGOUP_A, DPORT_A, D_ON);
        GPIO_WriteBit (DGOUP_B, DPORT_B, D_ON);
        GPIO_WriteBit (DGOUP_C, DPORT_C, D_ON);
        GPIO_WriteBit (DGOUP_D, DPORT_D, D_ON);
        GPIO_WriteBit (DGOUP_E, DPORT_E, D_ON);
        GPIO_WriteBit (DGOUP_F, DPORT_F, D_ON);
        GPIO_WriteBit (DGOUP_G, DPORT_G, D_OFF);
        GPIO_WriteBit (DGOUP_DP, DPORT_DP, D_OFF);
      };
      break;
    case 1: //
      {
        GPIO_WriteBit (DGOUP_A, DPORT_A, D_OFF);
        GPIO_WriteBit (DGOUP_B, DPORT_B, D_ON);
        GPIO_WriteBit (DGOUP_C, DPORT_C, D_ON);
        GPIO_WriteBit (DGOUP_D, DPORT_D, D_OFF);
        GPIO_WriteBit (DGOUP_E, DPORT_E, D_OFF);
        GPIO_WriteBit (DGOUP_F, DPORT_F, D_OFF);
        GPIO_WriteBit (DGOUP_G, DPORT_G, D_OFF);
        GPIO_WriteBit (DGOUP_DP, DPORT_DP, D_OFF);
      };
      break;
    case 2: //
      {
        GPIO_WriteBit (DGOUP_A, DPORT_A, D_ON);
        GPIO_WriteBit (DGOUP_B, DPORT_B, D_ON);
        GPIO_WriteBit (DGOUP_C, DPORT_C, D_OFF);
        GPIO_WriteBit (DGOUP_D, DPORT_D, D_ON);
        GPIO_WriteBit (DGOUP_E, DPORT_E, D_ON);
        GPIO_WriteBit (DGOUP_F, DPORT_F, D_OFF);
        GPIO_WriteBit (DGOUP_G, DPORT_G, D_ON);
        GPIO_WriteBit (DGOUP_DP, DPORT_DP, D_OFF);
      };
      break;
    case 3: //
      {
        GPIO_WriteBit (DGOUP_A, DPORT_A, D_ON);
        GPIO_WriteBit (DGOUP_B, DPORT_B, D_ON);
        GPIO_WriteBit (DGOUP_C, DPORT_C, D_ON);
        GPIO_WriteBit (DGOUP_D, DPORT_D, D_ON);
        GPIO_WriteBit (DGOUP_E, DPORT_E, D_OFF);
        GPIO_WriteBit (DGOUP_F, DPORT_F, D_OFF);
        GPIO_WriteBit (DGOUP_G, DPORT_G, D_ON);
        GPIO_WriteBit (DGOUP_DP, DPORT_DP, D_OFF);
      };
      break;
    case 4: //
      {
        GPIO_WriteBit (DGOUP_A, DPORT_A, D_OFF);
        GPIO_WriteBit (DGOUP_B, DPORT_B, D_ON);
        GPIO_WriteBit (DGOUP_C, DPORT_C, D_ON);
        GPIO_WriteBit (DGOUP_D, DPORT_D, D_OFF);
        GPIO_WriteBit (DGOUP_E, DPORT_E, D_OFF);
        GPIO_WriteBit (DGOUP_F, DPORT_F, D_ON);
        GPIO_WriteBit (DGOUP_G, DPORT_G, D_ON);
        GPIO_WriteBit (DGOUP_DP, DPORT_DP, D_OFF);
      };
      break;
    case 5: //
      {
        GPIO_WriteBit (DGOUP_A, DPORT_A, D_ON);
        GPIO_WriteBit (DGOUP_B, DPORT_B, D_OFF);
        GPIO_WriteBit (DGOUP_C, DPORT_C, D_ON);
        GPIO_WriteBit (DGOUP_D, DPORT_D, D_ON);
        GPIO_WriteBit (DGOUP_E, DPORT_E, D_OFF);
        GPIO_WriteBit (DGOUP_F, DPORT_F, D_ON);
        GPIO_WriteBit (DGOUP_G, DPORT_G, D_ON);
        GPIO_WriteBit (DGOUP_DP, DPORT_DP, D_OFF);
      };
      break;
    case 6: //
      {
        GPIO_WriteBit (DGOUP_A, DPORT_A, D_ON);
        GPIO_WriteBit (DGOUP_B, DPORT_B, D_OFF);
        GPIO_WriteBit (DGOUP_C, DPORT_C, D_ON);
        GPIO_WriteBit (DGOUP_D, DPORT_D, D_ON);
        GPIO_WriteBit (DGOUP_E, DPORT_E, D_ON);
        GPIO_WriteBit (DGOUP_F, DPORT_F, D_ON);
        GPIO_WriteBit (DGOUP_G, DPORT_G, D_ON);
        GPIO_WriteBit (DGOUP_DP, DPORT_DP, D_OFF);
      };
      break;
    case 7: //
      {
        GPIO_WriteBit (DGOUP_A, DPORT_A, D_ON);
        GPIO_WriteBit (DGOUP_B, DPORT_B, D_ON);
        GPIO_WriteBit (DGOUP_C, DPORT_C, D_ON);
        GPIO_WriteBit (DGOUP_D, DPORT_D, D_OFF);
        GPIO_WriteBit (DGOUP_E, DPORT_E, D_OFF);
        GPIO_WriteBit (DGOUP_F, DPORT_F, D_OFF);
        GPIO_WriteBit (DGOUP_G, DPORT_G, D_OFF);
        GPIO_WriteBit (DGOUP_DP, DPORT_DP, D_OFF);
      };
      break;
    case 8: //
      {
        GPIO_WriteBit (DGOUP_A, DPORT_A, D_ON);
        GPIO_WriteBit (DGOUP_B, DPORT_B, D_ON);
        GPIO_WriteBit (DGOUP_C, DPORT_C, D_ON);
        GPIO_WriteBit (DGOUP_D, DPORT_D, D_ON);
        GPIO_WriteBit (DGOUP_E, DPORT_E, D_ON);
        GPIO_WriteBit (DGOUP_F, DPORT_F, D_ON);
        GPIO_WriteBit (DGOUP_G, DPORT_G, D_ON);
        GPIO_WriteBit (DGOUP_DP, DPORT_DP, D_OFF);
      };
      break;
    case 9: //
      {
        GPIO_WriteBit (DGOUP_A, DPORT_A, D_ON);
        GPIO_WriteBit (DGOUP_B, DPORT_B, D_ON);
        GPIO_WriteBit (DGOUP_C, DPORT_C, D_ON);
        GPIO_WriteBit (DGOUP_D, DPORT_D, D_ON);
        GPIO_WriteBit (DGOUP_E, DPORT_E, D_OFF);
        GPIO_WriteBit (DGOUP_F, DPORT_F, D_ON);
        GPIO_WriteBit (DGOUP_G, DPORT_G, D_ON);
        GPIO_WriteBit (DGOUP_DP, DPORT_DP, D_OFF);
      };
      break;
    }
}

static u8 i_off = 0;

static void
lcd5643printDigit (u8 i_pos, u8 i_num, u8 iMode)
{
  if (i_pos > OUT_DIG_4 && i_off == 1)
    return; //
            /*
             */

  i_off = 0;
  switch (i_pos)
    {
    case OUT_SEC_IND: // sec indicator, 0=9,11.7.4.2.1.10.5 1=12,8,6 - i_num=3
      {
        if (iMode == MODE_CLOCK)
          {
            GPIO_WriteBit (DGOUP_A, DPORT_A, D_OFF);
            GPIO_WriteBit (DGOUP_B, DPORT_B, D_OFF);
            GPIO_WriteBit (DGOUP_C, DPORT_C, D_OFF);
            GPIO_WriteBit (DGOUP_D, DPORT_D, D_OFF);
            GPIO_WriteBit (DGOUP_E, DPORT_E, D_OFF);
            GPIO_WriteBit (DGOUP_F, DPORT_F, D_OFF);
            GPIO_WriteBit (DGOUP_G, DPORT_G, D_OFF);
            if (i_num)
              {
                GPIO_WriteBit (DGOUP_DP, DPORT_DP, D_ON);
                GPIO_WriteBit (DGOUP_Dig1, DPORT_Dig1, D_ON);
                GPIO_WriteBit (DGOUP_Dig2, DPORT_Dig2, D_OFF);
                GPIO_WriteBit (DGOUP_Dig3, DPORT_Dig3, D_ON);
                GPIO_WriteBit (DGOUP_Dig4, DPORT_Dig4, D_ON);
              }
            else
              {
                GPIO_WriteBit (DGOUP_DP, DPORT_DP, D_OFF);
                GPIO_WriteBit (DGOUP_Dig1, DPORT_Dig1, D_ON);
                GPIO_WriteBit (DGOUP_Dig2, DPORT_Dig2, D_ON);
                GPIO_WriteBit (DGOUP_Dig3, DPORT_Dig3, D_ON);
                GPIO_WriteBit (DGOUP_Dig4, DPORT_Dig4, D_ON);
              }
          }
      };
      break;

    case OUT_SIG_1: // digit 1, 0=12 1=9,8,6 - i_num=11.7.4.2.1.10.5
      {
        if (iMode == MODE_CLOCK) // case OUT_DIG_1: //digit 1, 0=12 1=9,8,6 -
                                 // i_num=11.7.4.2.1.10.5
          {
            reg_set_num (i_num);
          }
        else
          {
            GPIO_WriteBit (DGOUP_A, DPORT_A, D_OFF);
            GPIO_WriteBit (DGOUP_B, DPORT_B, D_OFF);
            GPIO_WriteBit (DGOUP_C, DPORT_C, D_OFF);
            GPIO_WriteBit (DGOUP_D, DPORT_D, D_OFF);
            GPIO_WriteBit (DGOUP_E, DPORT_E, D_OFF);
            GPIO_WriteBit (DGOUP_F, DPORT_F, D_OFF);
            GPIO_WriteBit (DGOUP_G, DPORT_G, D_ON);
            GPIO_WriteBit (DGOUP_DP, DPORT_DP, D_OFF);
          }
        GPIO_WriteBit (DGOUP_Dig1, DPORT_Dig1, D_OFF);
        GPIO_WriteBit (DGOUP_Dig2, DPORT_Dig2, D_ON);
        GPIO_WriteBit (DGOUP_Dig3, DPORT_Dig3, D_ON);
        GPIO_WriteBit (DGOUP_Dig4, DPORT_Dig4, D_ON);
      };
      break;

    case OUT_DIG_2: // digit 2, 0=9 1=12,8,6 - i_num=11.7.4.2.1.10.5
      {
        reg_set_num (i_num);
        GPIO_WriteBit (DGOUP_Dig1, DPORT_Dig1, D_ON);
        GPIO_WriteBit (DGOUP_Dig2, DPORT_Dig2, D_OFF);
        GPIO_WriteBit (DGOUP_Dig3, DPORT_Dig3, D_ON);
        GPIO_WriteBit (DGOUP_Dig4, DPORT_Dig4, D_ON);
      };
      break;
    case OUT_DIG_3: // digit 3, 0=8 1=12,9,6 - i_num=11.7.4.2.1.10.5
      {
        reg_set_num (i_num);
        GPIO_WriteBit (DGOUP_Dig1, DPORT_Dig1, D_ON);
        GPIO_WriteBit (DGOUP_Dig2, DPORT_Dig2, D_ON);
        GPIO_WriteBit (DGOUP_Dig3, DPORT_Dig3, D_OFF);
        GPIO_WriteBit (DGOUP_Dig4, DPORT_Dig4, D_ON);
      };
      break;

    case OUT_C_4: // digit 4, 0=6 1=12,9,8 - i_num=11.7.4.2.1.10.5
      {
        if (iMode == MODE_CLOCK) // case OUT_DIG_4: //digit 4, 0=6 1=12,9,8 -
                                 // i_num=11.7.4.2.1.10.5
          {
            reg_set_num (i_num);
          }
        else
          {
            GPIO_WriteBit (DGOUP_A, DPORT_A, D_ON);
            GPIO_WriteBit (DGOUP_B, DPORT_B, D_OFF);
            GPIO_WriteBit (DGOUP_C, DPORT_C, D_OFF);
            GPIO_WriteBit (DGOUP_D, DPORT_D, D_ON);
            GPIO_WriteBit (DGOUP_E, DPORT_E, D_ON);
            GPIO_WriteBit (DGOUP_F, DPORT_F, D_ON);
            GPIO_WriteBit (DGOUP_G, DPORT_G, D_OFF);
            GPIO_WriteBit (DGOUP_DP, DPORT_DP, D_OFF);
          }
        GPIO_WriteBit (DGOUP_Dig1, DPORT_Dig1, D_ON);
        GPIO_WriteBit (DGOUP_Dig2, DPORT_Dig2, D_ON);
        GPIO_WriteBit (DGOUP_Dig3, DPORT_Dig3, D_ON);
        GPIO_WriteBit (DGOUP_Dig4, DPORT_Dig4, D_OFF);
      };
      break;

    default: // off ligth
      {
        GPIO_WriteBit (DGOUP_Dig1, DPORT_Dig1, D_ON);
        GPIO_WriteBit (DGOUP_Dig2, DPORT_Dig2, D_ON);
        GPIO_WriteBit (DGOUP_Dig3, DPORT_Dig3, D_ON);
        GPIO_WriteBit (DGOUP_Dig4, DPORT_Dig4, D_ON);
        GPIO_WriteBit (DGOUP_A, DPORT_A, D_OFF);
        GPIO_WriteBit (DGOUP_B, DPORT_B, D_OFF);
        GPIO_WriteBit (DGOUP_C, DPORT_C, D_OFF);
        GPIO_WriteBit (DGOUP_D, DPORT_D, D_OFF);
        GPIO_WriteBit (DGOUP_E, DPORT_E, D_OFF);
        GPIO_WriteBit (DGOUP_F, DPORT_F, D_OFF);
        GPIO_WriteBit (DGOUP_G, DPORT_G, D_OFF);
        GPIO_WriteBit (DGOUP_DP, DPORT_DP, D_OFF);
        i_off = 1;
      };
      break;
    }
}

u8 i_5643_hour = 0;
u8 i_5643_min = 0;
u8 i_5643_sec_state = 0;
u8 i_5643_Mode = MODE_CLOCK;
int i_5643_t_sign = 0;
int i_5643_t_value = 88;
int i_5643_t_mantissa = 0;

u8 i_5643_out = 0;
u8 i_5643_max_out = LCD_VAL_LG_hi; //

void
lcd5643_update_disp (void) // здесь будет вывод на LCD
{
  //        printf("timer irq hour=%d,min=%d\n",i_5643_hour,i_5643_min);
  u8 i_HiHour;
  u8 i_LoHour;
  u8 i_HiMin;
  u8 i_LoMin;

  if (i_5643_Mode == MODE_CLOCK)
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

  switch (i_5643_out)
    {
    case 0:
      {
        if (i_5643_Mode == MODE_CLOCK)
          lcd5643printDigit (OUT_DIG_1, i_HiHour, i_5643_Mode);
        else
          {
            if (i_5643_t_sign > 0)
              lcd5643printDigit (OUT_SIG_1, 1, i_5643_Mode); // "+"
            else
              lcd5643printDigit (OUT_SIG_1, 0, i_5643_Mode); // "-"
          }
      };
      break;
    case 1:
      {
        if (i_5643_Mode == MODE_CLOCK)
          lcd5643printDigit (OUT_DIG_2, i_LoHour, i_5643_Mode);
        else
          lcd5643printDigit (OUT_DIG_2, i_t / 10, i_5643_Mode);
      };
      break;
    case 2:
      {
        if (i_5643_Mode == MODE_CLOCK)
          lcd5643printDigit (OUT_DIG_3, i_HiMin, i_5643_Mode);
        else
          lcd5643printDigit (OUT_DIG_3, i_t % 10, i_5643_Mode);
      };
      break;
    case 3:
      {
        if (i_5643_Mode == MODE_CLOCK)
          lcd5643printDigit (OUT_DIG_4, i_LoMin, i_5643_Mode);
        else
          lcd5643printDigit (OUT_C_4, 0, i_5643_Mode);
      };
      break;
    case 4:
      {
        if (i_5643_Mode == MODE_CLOCK)
          lcd5643printDigit (OUT_SEC_IND, i_5643_sec_state,
                             i_5643_Mode); // on sec state
      };
      break;
    default:
      {
        lcd5643printDigit (
            5, 0,
            i_5643_Mode); // off  пока i_5643_out будет больше 4, - выключить LCD
      };
      break;
    }

  if (i_5643_out++ > i_5643_max_out) // от 5 ...
    {
      i_5643_out = 0;
    }
}
