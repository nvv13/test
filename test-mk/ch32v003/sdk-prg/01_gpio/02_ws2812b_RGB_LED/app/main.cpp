/*
  источник
  https://github.com/openwch/ch32v003.git
*/

#include <string.h>

#include "debug.h"
#include "ws2812b.h"
#include "ws2812b_params.h"

#include "CUsart.hpp"

#define US_PER_MS 1

/**
 * @brief   Switch to the next LED every 10ms
 */
#define STEP (10 * US_PER_MS)

/**
 * @brief   Interval for dimming colors
 */
#define DIM (5 * US_PER_MS)

/**
 * @brief   Step through brightness levels (0-255) in 32 steps
 */
#define BSTEP (8U)

/**
 * @brief   Allocate the device descriptor
 */
static ws2812b_t dev;

/**
 * @brief   Allocate a color_rgb_t struct for each LED on the strip
 */
static color_rgba_t leds[WS2812B_PARAM_LED_NUMOF];

static void
setcolor (int color, uint8_t alpha)
{
  for (int i = 0; i < (int)WS2812B_PARAM_LED_NUMOF; i++)
    {
      leds[i].alpha = alpha;
      memset (&leds[i].color, 0, sizeof (color_rgb_t));
      switch (color)
        {
        case 0:
          leds[i].color.r = 255;
          break;
        case 1:
          leds[i].color.g = 255;
          break;
        case 2:
          leds[i].color.b = 255;
          break;
        }
    }
}

extern "C" int
main (void)
{

  NVIC_PriorityGroupConfig (NVIC_PriorityGroup_1);
  Delay_Init ();
  Delay_Ms (100);
  SystemCoreClockUpdate ();

  CUsart *oUsart = CUsart::GetInstance ();
  oUsart->SendPSZstring ("SystemClk1:");
  oUsart->SendIntToStr (SystemCoreClock);
  oUsart->SendPSZstring ("\r\n");

  int pos = 0;
  int step = 1;
  color_hsv_t col = { 0.0, 1.0, 1.0 };

  oUsart->SendPSZstring ("WS2812B Test App\r\n");

  /* initialize all LED color values to black (off) */
  memset (leds, 0, sizeof (color_rgba_t) * WS2812B_PARAM_LED_NUMOF);

  ws2812b_params_t param;
  param.led_numof = WS2812B_PARAM_LED_NUMOF;
  param.data_pin = PC_01;

  ws2812b_init (&dev, &param);

  oUsart->SendPSZstring ("Initialization done.\r\n");

  /* test
  while (1)
    {
      setcolor (1, 123);
      ws2812b_load_rgba (&dev, leds);
    }
  */

  /* set to each red, green, and blue, and fade each color in and out */
  for (int col = 0; col <= 2; col++)
    {
      int i = 0;
      for (; i <= (int)UINT8_MAX; i += BSTEP)
        {
          setcolor (col, (uint8_t)i);
          ws2812b_load_rgba (&dev, leds);
          Delay_Ms (DIM);
        }
      i -= BSTEP;
      for (; i >= 0; i -= BSTEP)
        {
          setcolor (col, (uint8_t)i);
          ws2812b_load_rgba (&dev, leds);
          Delay_Ms (DIM);
        }
    }

  oUsart->SendPSZstring ("Color Fading done\r\n");

  /* reset color values */
  setcolor (-1, 255);
  ws2812b_load_rgba (&dev, leds);

  while (1)
    {
      /* change the active color by running around the hue circle */
      col.h += 1.0;
      if (col.h > 360.0)
        {
          col.h = 0.0;
        }

      /* set the active LED to the active color value */
      memset (&leds[pos].color, 0, sizeof (color_rgb_t));
      pos += step;
      color_hsv2rgb (&col, &leds[pos].color);

      /* apply the values to the LED strip */
      ws2812b_load_rgba (&dev, leds);

      /* switch direction once reaching an end of the strip */
      if ((pos == (WS2812B_PARAM_LED_NUMOF - 1)) || (pos == 0))
        {
          step *= -1;
        }

      Delay_Ms (STEP);
    }
}
