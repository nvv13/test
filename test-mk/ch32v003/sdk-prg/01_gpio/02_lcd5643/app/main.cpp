/*
  источник
  https://github.com/openwch/ch32v003.git
*/

#include <stdlib.h>

#include <cstring>

#include "debug.h"

#include "lcd5643.h"

int
main (void)
{
  NVIC_PriorityGroupConfig (NVIC_PriorityGroup_1);
  SystemCoreClockUpdate ();
  Delay_Init ();
  // USART_Printf_Init (115000);

  lcd5643_init_pin (MODE_COMMON_ANODE);

  int i_cnt_disp = 0;
  while (1)
    {
      Delay_Ms (1);
      if (i_cnt_disp++ > 1000)
        {
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
          i_5643_sec_state = ~i_5643_sec_state;
        }
      lcd5643_update_disp ();
    }
}
