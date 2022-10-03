#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"

#include "wm_osal.h"

#include "ws2812b.h"

#include "el_cmd.h"

extern ws2812b_t dev;

u32 u32_US_PER_MS = 1;

/**
 * @brief   Switch to the next LED every 10ms
 */
static u32 u32_STEP = 3;

/**
 * @brief   Interval for dimming colors
 */
static u32 u32_DIM = 3;

/**
 * @brief   Step through brightness levels (0-255) in 32 steps
 */
#define BSTEP (8U)

u32
ef_count_delay_STEP (void)
{
  u32_STEP = (3 * u32_US_PER_MS);
  u32_DIM =  (3 * u32_US_PER_MS);
  return u32_STEP;
}

/**
 * @brief   Allocate a color_rgb_t struct for each LED on the strip
 */
#define MAX_WS2812B_LED_NUMOF 400
static color_rgba_t leds[MAX_WS2812B_LED_NUMOF];

static void
setcolor (int color, uint8_t alpha)
{
  for (int i = 0; i < (int)dev.led_numof; i++)
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

void
el_init (void)
{
  /* initialize all LED color values to black (off) */
  memset (leds, 0, sizeof (color_rgba_t) * MAX_WS2812B_LED_NUMOF);
  /* reset color values */
  setcolor (-1, 255);
  ws2812b_load_rgba (&dev, leds);
}

void
ef_1 (void)
{
  /* set to each red, green, and blue, and fade each color in and out */
  for (int col = 0; col <= 2; col++)
    {
      int i = 0;
      for (; i <= (int)UINT8_MAX; i += BSTEP)
        {
          setcolor (col, (uint8_t)i);
          ws2812b_load_rgba (&dev, leds);
          tls_os_time_delay (u32_DIM);
        }
      i -= BSTEP;
      for (; i >= 0; i -= BSTEP)
        {
          setcolor (col, (uint8_t)i);
          ws2812b_load_rgba (&dev, leds);
          tls_os_time_delay (u32_DIM);
        }
    }

  /* reset color values */
  setcolor (-1, 255);
  ws2812b_load_rgba (&dev, leds);

  puts ("Color Fading done");
}

int pos = 0;
int step = 1;
color_hsv_t col = { 0.0, 1.0, 1.0 };

void
ef_2 (void)
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
  if ((pos == (dev.led_numof - 1)) || (pos == 0))
    {
      step *= -1;
    }

  tls_os_time_delay (u32_STEP);

  puts ("ef_2 done");
}
