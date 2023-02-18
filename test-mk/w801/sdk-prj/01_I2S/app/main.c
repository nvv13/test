#include "wm_type_def.h"

#include "wm_i2c.h"
#include "wm_regs.h"

#include "wm_gpio.h"
#include "wm_pmu.h"

#include <stdio.h>
#include <string.h>

#include "wm_gpio_afsel.h"

void
UserMain (void)
{

   wm_i2s_ck_config(WM_IO_PA_08); // clock line         i2s Bclk
   wm_i2s_ws_config(WM_IO_PA_09); // word select line   i2s LRclk
   wm_i2s_do_config(WM_IO_PA_10); // Dout
   wm_i2s_di_config(WM_IO_PA_11); // Din


  //wm_i2s_mclk_config (WM_IO_PA_00); // only PA0  ,The clock is still the internal 160MHz clock, whether the MCLK clock is turned on, and the operating frequencies of MCLK and BCLK
  //wm_i2s_extclk_config (WM_IO_PA_07); // only PA7,The working clock configuration of the I2S module can be configured by setting the 0x40000718 register in the clock and reset module to select the external clock source.

  while (1)
    {
    };
}
