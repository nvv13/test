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
#include "wm_cpu.h"
#include "wm_regs.h"
#include "wm_rtc.h"
#include "wm_timer.h"
#include "wm_watchdog.h"
//#include "csi_core.h"

#include "mod1/u8g2.h"
#include "u8x8_riotos.h"

/**
 * @brief   RIOT-OS logo, 64x32 pixels at 8 pixels per byte.
 */
static const uint8_t logo[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x3C, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x78, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x0E,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xF0, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x0E, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xF0, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0,
  0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x7C, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x73, 0xF8, 0x30, 0x3C, 0x3F, 0xC0, 0x00, 0x0C, 0x77, 0xF0, 0x38,
  0x7E, 0x3F, 0xC0, 0x00, 0x7E, 0x73, 0xC0, 0x38, 0xE7, 0x06, 0x00, 0x00, 0xFC,
  0x71, 0x00, 0x38, 0xE3, 0x06, 0x00, 0x01, 0xF0, 0x70, 0x00, 0x38, 0xE3, 0x06,
  0x00, 0x01, 0xC0, 0x70, 0x00, 0x38, 0xE3, 0x06, 0x00, 0x03, 0x80, 0x70, 0xC0,
  0x38, 0xE3, 0x06, 0x00, 0x03, 0x80, 0x71, 0xE0, 0x38, 0xE3, 0x06, 0x00, 0x03,
  0x80, 0x70, 0xE0, 0x38, 0xE3, 0x06, 0x00, 0x03, 0x80, 0x70, 0xF0, 0x38, 0xE3,
  0x06, 0x00, 0x03, 0x80, 0x70, 0x70, 0x38, 0xE3, 0x06, 0x00, 0x03, 0x80, 0xF0,
  0x78, 0x38, 0xE3, 0x06, 0x00, 0x03, 0xC1, 0xE0, 0x3C, 0x38, 0xE7, 0x06, 0x00,
  0x01, 0xE3, 0xE0, 0x3C, 0x38, 0x7E, 0x06, 0x00, 0x01, 0xFF, 0xC0, 0x1C, 0x30,
  0x3C, 0x06, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*
LCD	       - W801
--------------- --------------------------------
1 GND          - GND
2 VCC (+5V)    - +5V
3 Vo
4 RS (CS)      - PB21 (spi1 nss)
5 R/W (MOSI)   - PB17 (spi1 mosi)
6 E (SCK)      - PB15 (spi1 sck)
7  DB0
8  DB1
9  DB2
10 DB3
11 DB4
12 DB5
13 DB6
14 DB7
15 PSB         - GND (0- serial input/1-parralel)
16 NC
17 RST         - PB22
18 Vout
19 BLA         - +3.3v
20 BLK         - GND
*/

void
UserMain (void)
{
  printf ("user task\n");
  // tls_sys_clk_set(CPU_CLK_240M);

  uint32_t screen = 0;
  u8g2_t u8g2;

  /* initialize to SPI */
  puts ("Initializing to I2C.");

  u8g2_Setup_sh1106_i2c_128x64_noname_f (
      &u8g2, U8G2_R0, u8x8_byte_hw_i2c_riotos, u8x8_gpio_and_delay_riotos);

  // void u8g2_Setup_sh1106_i2c_128x64_noname_1(u8g2_t *u8g2, const u8g2_cb_t
  // *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb); void
  // u8g2_Setup_sh1106_i2c_128x64_vcomh0_1(u8g2_t *u8g2, const u8g2_cb_t
  // *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb); void
  // u8g2_Setup_sh1106_i2c_128x64_winstar_1(u8g2_t *u8g2, const u8g2_cb_t
  // *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb); void
  // u8g2_Setup_sh1106_i2c_128x64_noname_2(u8g2_t *u8g2, const u8g2_cb_t
  // *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb); void
  // u8g2_Setup_sh1106_i2c_128x64_vcomh0_2(u8g2_t *u8g2, const u8g2_cb_t
  // *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb); void
  // u8g2_Setup_sh1106_i2c_128x64_winstar_2(u8g2_t *u8g2, const u8g2_cb_t
  // *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb); void
  // u8g2_Setup_sh1106_i2c_128x64_noname_f(u8g2_t *u8g2, const u8g2_cb_t
  // *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb); void
  // u8g2_Setup_sh1106_i2c_128x64_vcomh0_f(u8g2_t *u8g2, const u8g2_cb_t
  // *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb); void
  // u8g2_Setup_sh1106_i2c_128x64_winstar_f(u8g2_t *u8g2, const u8g2_cb_t
  // *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb);

  if (u8g2.u8x8.i2c_address == 255)
    u8g2.u8x8.i2c_address = 0x3C;

  u8x8_riotos_t user_data = {
    .pin_cs = GPIO_UNDEF,    //
    .pin_dc = GPIO_UNDEF,    //
    .pin_reset = GPIO_UNDEF, //

    .i2c_scl = WM_IO_PA_01, /* */
    .i2c_sda = WM_IO_PA_04, /* */
    .i2c_freq = 100000      /* частота i2c в герцах */

  };

  u8g2_SetUserPtr (&u8g2, &user_data);

  /* initialize the display */
  puts ("Initializing display.");

  u8g2_InitDisplay (&u8g2);
  u8g2_SetPowerSave (&u8g2, 0);

  /* start drawing in a loop */
  puts ("Drawing on screen.");

  while (1)
    {
      u8g2_FirstPage (&u8g2);

      do
        {
          u8g2_SetDrawColor (&u8g2, 1);
          u8g2_SetFont (&u8g2, u8g2_font_helvB12_tf);

          switch (screen)
            {
            case 0:
              u8g2_DrawStr (&u8g2, 12, 22, "THIS");
              break;
            case 1:
              u8g2_DrawStr (&u8g2, 24, 22, "IS");
              break;
            case 2:
              u8g2_DrawBitmap (&u8g2, 0, 0, 8, 32, logo);
              break;
            case 4:
              u8g2_DrawStr (&u8g2, 24, 22, "Vova Hi !");
              break;
            case 5:
              u8g2_DrawStr (&u8g2, 4, 42, "Everything Ok !");
              break;
            case 6:
              u8g2_DrawDisc (&u8g2, 10, 18, 9, U8G2_DRAW_ALL);
              u8g2_DrawCircle (&u8g2, 10, 18 + 30, 9, U8G2_DRAW_ALL);
              u8g2_DrawLine (&u8g2, 7, 10, 40, 55);
              u8g2_DrawBox (&u8g2, 50, 10, 20, 10);
              u8g2_DrawFrame (&u8g2, 50, 10 + 30, 20, 10);
              break;
            }
        }
      while (u8g2_NextPage (&u8g2));

      /* show screen in next iteration */
      // screen = (screen + 1) % 3;
      if (++screen > 6)
        screen = 0;

      /* sleep a little */
      tls_os_time_delay (HZ);
    }
}
