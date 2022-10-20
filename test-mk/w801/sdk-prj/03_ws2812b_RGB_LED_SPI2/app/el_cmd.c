#include <math.h>
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
static u32 u32_STEP = 6;

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
  u32_STEP = (6 * u32_US_PER_MS);
  u32_DIM = (3 * u32_US_PER_MS);
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

static void
one_color_all (u8 r, u8 g, u8 b)
{ //-SET ALL LEDS TO ONE COLOR (r g b)
  for (int i = 0; i < dev.led_numof; i++)
    {
      leds[i].color.r = r;
      leds[i].color.g = g;
      leds[i].color.b = b;
    }
}

static int TOP_INDEX = 0;
static int EVENODD = 0;
static int max_bright = 51; // максимальная яркость (0 - 255)
void
el_init (void)
{
  /* initialize all LED color values to black (off) */
  memset (leds, 0, sizeof (color_rgba_t) * MAX_WS2812B_LED_NUMOF);
  /* reset color values */
  setcolor (-1, max_bright);
  ws2812b_load_rgba (&dev, leds);

  TOP_INDEX = dev.led_numof / 2;
  EVENODD = dev.led_numof % 2;
  ef_count_delay_STEP ();
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
  setcolor (-1, max_bright);
  ws2812b_load_rgba (&dev, leds);

  //  puts ("Color Fading done");
}

static int pos = 0;
static int step = 1;
static color_hsv_t col = { 0.0, 1.0, 1.0 };

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

  //  puts ("ef_2 done");
}

volatile static int ledMode = 3;
/*
  Стартовый режим
  0 - все выключены
  1 - все включены
  3 - кольцевая радуга
  888 - демо-режим
*/

// цвета мячиков для режима
static u8 ballColors[3][3] = {
  { 0xff, 0, 0 },
  { 0xff, 0xff, 0xff },
  { 0, 0, 0xff },
};
// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------
static int BOTTOM_INDEX = 0; // светодиод начала отсчёта
static uint8_t ledsX[MAX_WS2812B_LED_NUMOF]
                    [3]; //-ARRAY FOR COPYING WHATS IN THE LED STRIP CURRENTLY
                         //(FOR CELL-AUTOMATA, MARCH, ETC)

static int thisdelay = 20; //-FX LOOPS DELAY VAR
static int thisstep = 10;  //-FX LOOPS DELAY VAR
static int thishue = 0;    //-FX LOOPS DELAY VAR
static int thissat = 255;  //-FX LOOPS DELAY VAR

// static int thisindex = 0;
// static int thisRED = 0;
// static int thisGRN = 0;
// static int thisBLU = 0;

static int idex = 0;            //-LED INDEX (0 to LED_COUNT-1
static int ihue = 0;            //-HUE (0-255)
static int ibright = 0;         //-BRIGHTNESS (0-255)
static int isat = 0;            //-SATURATION (0-255)
static int bouncedirection = 0; //-SWITCH FOR COLOR BOUNCE (0-1)
static float tcount = 0.0;      //-INC VAR FOR SIN LOOPS
static int lcount = 0;          //-ANOTHER COUNTING VAR
// ---------------СЛУЖЕБНЫЕ ПЕРЕМЕННЫЕ-----------------

static void
CHSV (color_rgb_t *rgb, float h, float s, float v)
{
  color_hsv_t hsv;
  hsv.h = h;
  hsv.s = s;
  hsv.v = v;
  color_hsv2rgb (&hsv, rgb);
  return;
}

//------------------------------------- UTILITY FXNS
//--------------------------------------
//---SET THE COLOR OF A SINGLE RGB LED
static void
set_color_led (int adex, int cred, int cgrn, int cblu)
{
  leds[adex].color.r = cred;
  leds[adex].color.g = cgrn;
  leds[adex].color.b = cblu;
}

//---FIND INDEX OF HORIZONAL OPPOSITE LED
static int
horizontal_index (int i)
{
  //-ONLY WORKS WITH INDEX < TOPINDEX
  if (i == BOTTOM_INDEX)
    {
      return BOTTOM_INDEX;
    }
  if (i == TOP_INDEX && EVENODD == 1)
    {
      return TOP_INDEX + 1;
    }
  if (i == TOP_INDEX && EVENODD == 0)
    {
      return TOP_INDEX;
    }
  return dev.led_numof - i;
}

//---FIND INDEX OF ANTIPODAL OPPOSITE LED
static int
antipodal_index (int i)
{
  int iN = i + TOP_INDEX;
  if (i >= TOP_INDEX)
    {
      iN = (i + TOP_INDEX) % dev.led_numof;
    }
  return iN;
}

//---FIND ADJACENT INDEX CLOCKWISE
static int
adjacent_cw (int i)
{
  int r;
  if (i < dev.led_numof - 1)
    {
      r = i + 1;
    }
  else
    {
      r = 0;
    }
  return r;
}

//---FIND ADJACENT INDEX COUNTER-CLOCKWISE
static int
adjacent_ccw (int i)
{
  int r;
  if (i > 0)
    {
      r = i - 1;
    }
  else
    {
      r = dev.led_numof - 1;
    }
  return r;
}

static void
copy_led_array ()
{
  for (int i = 0; i < dev.led_numof; i++)
    {
      ledsX[i][0] = leds[i].color.r;
      ledsX[i][1] = leds[i].color.g;
      ledsX[i][2] = leds[i].color.b;
    }
}

static void
setPixel (int Pixel, u8 red, u8 green, u8 blue)
{
  leds[Pixel].color.r = red;
  leds[Pixel].color.g = green;
  leds[Pixel].color.b = blue;
}

static void
setAll (u8 red, u8 green, u8 blue)
{
  for (int i = 0; i < dev.led_numof; i++)
    {
      setPixel (i, red, green, blue);
    }
  ws2812b_load_rgba (&dev, leds);
}

//-----------------------------------------------

static void
rainbow_fade ()
{ //-m2-FADE ALL LEDS THROUGH HSV RAINBOW
  ihue++;
  if (ihue > 255)
    {
      ihue = 0;
    }
  for (int idex = 0; idex < dev.led_numof; idex++)
    {
      CHSV (&leds[idex].color, ihue, thissat, 255);
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
rainbow_loop ()
{ //-m3-LOOP HSV RAINBOW
  idex++;
  ihue = ihue + thisstep;
  if (idex >= dev.led_numof)
    {
      idex = 0;
    }
  if (ihue > 255)
    {
      ihue = 0;
    }
  CHSV (&leds[idex].color, ihue, thissat, 255);
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static int
random (int min_num, int max_num)
{
  int result = 0, low_num = 0, hi_num = 0;

  if (min_num < max_num)
    {
      low_num = min_num;
      hi_num = max_num + 1; // include max_num in output
    }
  else
    {
      low_num = max_num + 1; // include max_num in output
      hi_num = min_num;
    }

  srand (tls_os_get_time ()); // time(NULL));
  result = (rand () % (hi_num - low_num)) + low_num;
  return result;
}

static void
random_burst ()
{ //-m4-RANDOM INDEX/COLOR
  idex = random (0, dev.led_numof);
  ihue = random (0, 255);
  CHSV (&leds[idex].color, ihue, thissat, 255);
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
color_bounce ()
{ //-m5-BOUNCE COLOR (SINGLE LED)
  if (bouncedirection == 0)
    {
      idex = idex + 1;
      if (idex == dev.led_numof)
        {
          bouncedirection = 1;
          idex = idex - 1;
        }
    }
  if (bouncedirection == 1)
    {
      idex = idex - 1;
      if (idex == 0)
        {
          bouncedirection = 0;
        }
    }
  for (int i = 0; i < dev.led_numof; i++)
    {
      if (i == idex)
        {
          CHSV (&leds[idex].color, thishue, thissat, 255);
        }
      else
        {
          CHSV (&leds[idex].color, 0, 0, 0);
        }
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
color_bounceFADE ()
{ //-m6-BOUNCE COLOR (SIMPLE MULTI-LED FADE)
  if (bouncedirection == 0)
    {
      idex = idex + 1;
      if (idex == dev.led_numof)
        {
          bouncedirection = 1;
          idex = idex - 1;
        }
    }
  if (bouncedirection == 1)
    {
      idex = idex - 1;
      if (idex == 0)
        {
          bouncedirection = 0;
        }
    }
  int iL1 = adjacent_cw (idex);
  int iL2 = adjacent_cw (iL1);
  int iL3 = adjacent_cw (iL2);
  int iR1 = adjacent_ccw (idex);
  int iR2 = adjacent_ccw (iR1);
  int iR3 = adjacent_ccw (iR2);
  for (int i = 0; i < dev.led_numof; i++)
    {
      if (i == idex)
        {
          CHSV (&leds[i].color, thishue, thissat, 255);
        }
      else if (i == iL1)
        {
          CHSV (&leds[i].color, thishue, thissat, 150);
        }
      else if (i == iL2)
        {
          CHSV (&leds[i].color, thishue, thissat, 80);
        }
      else if (i == iL3)
        {
          CHSV (&leds[i].color, thishue, thissat, 20);
        }
      else if (i == iR1)
        {
          CHSV (&leds[i].color, thishue, thissat, 150);
        }
      else if (i == iR2)
        {
          CHSV (&leds[i].color, thishue, thissat, 80);
        }
      else if (i == iR3)
        {
          CHSV (&leds[i].color, thishue, thissat, 20);
        }
      else
        {
          CHSV (&leds[i].color, 0, 0, 0);
        }
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
ems_lightsONE ()
{ //-m7-EMERGENCY LIGHTS (TWO COLOR SINGLE LED)
  idex++;
  if (idex >= dev.led_numof)
    {
      idex = 0;
    }
  int idexR = idex;
  int idexB = antipodal_index (idexR);
  int thathue = (thishue + 160) % 255;
  for (int i = 0; i < dev.led_numof; i++)
    {
      if (i == idexR)
        {
          CHSV (&leds[i].color, thishue, thissat, 255);
        }
      else if (i == idexB)
        {
          CHSV (&leds[i].color, thathue, thissat, 255);
        }
      else
        {
          CHSV (&leds[i].color, 0, 0, 0);
        }
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
ems_lightsALL ()
{ //-m8-EMERGENCY LIGHTS (TWO COLOR SOLID)
  idex++;
  if (idex >= dev.led_numof)
    {
      idex = 0;
    }
  int idexR = idex;
  int idexB = antipodal_index (idexR);
  int thathue = (thishue + 160) % 255;
  CHSV (&leds[idexR].color, thishue, thissat, 255);
  CHSV (&leds[idexB].color, thathue, thissat, 255);
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
flicker ()
{ //-m9-FLICKER EFFECT
  int random_bright = random (0, 255);
  int random_delay = random (10, 100);
  int random_bool = random (0, random_bright);
  if (random_bool < 10)
    {
      for (int i = 0; i < dev.led_numof; i++)
        {
          CHSV (&leds[i].color, thishue, thissat, random_bright);
        }
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (random_delay);
    }
}

static void
pulse_one_color_all ()
{ //-m10-PULSE BRIGHTNESS ON ALL LEDS TO ONE COLOR
  if (bouncedirection == 0)
    {
      ibright++;
      if (ibright >= 255)
        {
          bouncedirection = 1;
        }
    }
  if (bouncedirection == 1)
    {
      ibright = ibright - 1;
      if (ibright <= 1)
        {
          bouncedirection = 0;
        }
    }
  for (int idex = 0; idex < dev.led_numof; idex++)
    {
      CHSV (&leds[idex].color, thishue, thissat, ibright);
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
pulse_one_color_all_rev ()
{ //-m11-PULSE SATURATION ON ALL LEDS TO ONE COLOR
  if (bouncedirection == 0)
    {
      isat++;
      if (isat >= 255)
        {
          bouncedirection = 1;
        }
    }
  if (bouncedirection == 1)
    {
      isat = isat - 1;
      if (isat <= 1)
        {
          bouncedirection = 0;
        }
    }
  for (int idex = 0; idex < dev.led_numof; idex++)
    {
      CHSV (&leds[idex].color, thishue, isat, 255);
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
fade_vertical ()
{ //-m12-FADE 'UP' THE LOOP
  idex++;
  if (idex > TOP_INDEX)
    {
      idex = 0;
    }
  int idexA = idex;
  int idexB = horizontal_index (idexA);
  ibright = ibright + 10;
  if (ibright > 255)
    {
      ibright = 0;
    }
  CHSV (&leds[idexA].color, thishue, thissat, ibright);
  CHSV (&leds[idexB].color, thishue, thissat, ibright);
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
random_red ()
{ // QUICK 'N DIRTY RANDOMIZE TO GET CELL AUTOMATA STARTED
  int temprand;
  for (int i = 0; i < dev.led_numof; i++)
    {
      temprand = random (0, 100);
      if (temprand > 50)
        {
          leds[i].color.r = 255;
        }
      if (temprand <= 50)
        {
          leds[i].color.r = 0;
        }
      leds[i].color.b = 0;
      leds[i].color.g = 0;
    }
  ws2812b_load_rgba (&dev, leds);
}

static void
rule30 ()
{ //-m13-1D CELLULAR AUTOMATA - RULE 30 (RED FOR NOW)
  if (bouncedirection == 0)
    {
      random_red ();
      bouncedirection = 1;
    }
  copy_led_array ();
  int iCW;
  int iCCW;
  int y = 100;
  for (int i = 0; i < dev.led_numof; i++)
    {
      iCW = adjacent_cw (i);
      iCCW = adjacent_ccw (i);
      if (ledsX[iCCW][0] > y && ledsX[i][0] > y && ledsX[iCW][0] > y)
        {
          leds[i].color.r = 0;
        }
      if (ledsX[iCCW][0] > y && ledsX[i][0] > y && ledsX[iCW][0] <= y)
        {
          leds[i].color.r = 0;
        }
      if (ledsX[iCCW][0] > y && ledsX[i][0] <= y && ledsX[iCW][0] > y)
        {
          leds[i].color.r = 0;
        }
      if (ledsX[iCCW][0] > y && ledsX[i][0] <= y && ledsX[iCW][0] <= y)
        {
          leds[i].color.r = 255;
        }
      if (ledsX[iCCW][0] <= y && ledsX[i][0] > y && ledsX[iCW][0] > y)
        {
          leds[i].color.r = 255;
        }
      if (ledsX[iCCW][0] <= y && ledsX[i][0] > y && ledsX[iCW][0] <= y)
        {
          leds[i].color.r = 255;
        }
      if (ledsX[iCCW][0] <= y && ledsX[i][0] <= y && ledsX[iCW][0] > y)
        {
          leds[i].color.r = 255;
        }
      if (ledsX[iCCW][0] <= y && ledsX[i][0] <= y && ledsX[iCW][0] <= y)
        {
          leds[i].color.r = 0;
        }
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
random_march ()
{ //-m14-RANDOM MARCH CCW
  copy_led_array ();
  int iCCW;
  CHSV (&leds[0].color, random (0, 255), 255, 255);
  for (int idex = 1; idex < dev.led_numof; idex++)
    {
      iCCW = adjacent_ccw (idex);
      leds[idex].color.r = ledsX[iCCW][0];
      leds[idex].color.g = ledsX[iCCW][1];                                              		
      leds[idex].color.b = ledsX[iCCW][2];
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
rwb_march ()
{ //-m15-R,W,B MARCH CCW
  copy_led_array ();
  int iCCW;
  idex++;
  if (idex > 2)
    {
      idex = 0;
    }
  switch (idex)
    {
    case 0:
      leds[0].color.r = 255;
      leds[0].color.g = 0;
      leds[0].color.b = 0;
      break;
    case 1:
      leds[0].color.r = 255;
      leds[0].color.g = 255;
      leds[0].color.b = 255;
      break;
    case 2:
      leds[0].color.r = 0;
      leds[0].color.g = 0;
      leds[0].color.b = 255;
      break;
    }
  for (int i = 1; i < dev.led_numof; i++)
    {
      iCCW = adjacent_ccw (i);
      leds[i].color.r = ledsX[iCCW][0];
      leds[i].color.g = ledsX[iCCW][1];
      leds[i].color.b = ledsX[iCCW][2];
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
radiation ()
{ //-m16-SORT OF RADIATION SYMBOLISH-
  int N3 = (dev.led_numof / 3);
  int N6 = (dev.led_numof / 6);
  int N12 = (dev.led_numof / 12);
  for (int i = 0; i < N6; i++)
    { //-HACKY, I KNOW...
      tcount = tcount + .02;
      if (tcount > 3.14)
        {
          tcount = 0.0;
        }
      ibright = (sin (tcount) * 255);
      int j0 = (i + dev.led_numof - N12) % dev.led_numof;
      int j1 = (j0 + N3) % dev.led_numof;
      int j2 = (j1 + N3) % dev.led_numof;
      CHSV (&leds[j0].color, thishue, thissat, ibright);
      CHSV (&leds[j1].color, thishue, thissat, ibright);
      CHSV (&leds[j2].color, thishue, thissat, ibright);
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

/*
constrain(x, a, b)
Функция проверяет и если надо задает новое значение,
так чтобы оно была в области допустимых значений, заданной параметрами.

Параметры
x: проверяемое значение, любой тип
a: нижняя граница области допустимых значений, любой тип
b: верхняя граница области допустимых значений, любой тип
*/
static int
constrain (int x, int a, int b)
{
  if (x < a)
    return a;
  if (x > b)
    return b;
  return x;
}

static void
color_loop_vardelay ()
{ //-m17-COLOR LOOP (SINGLE LED) w/ VARIABLE DELAY
  idex++;
  if (idex > dev.led_numof)
    {
      idex = 0;
    }
  int di = abs (TOP_INDEX - idex);
  //printf (" color_loop_vardelay TOP_INDEX%d idex%d di%d\n",TOP_INDEX,idex, di);
  if(di==0)di=1;
  int t = constrain ((10 / di) * 10, 10, 500);
  for (int i = 0; i < dev.led_numof; i++)
    {
      if (i == idex)
        {
          CHSV (&leds[i].color, 0, thissat, 255);
        }
      else
        {
          leds[i].color.r = 0;
          leds[i].color.g = 0;
          leds[i].color.b = 0;
        }
    }
  ws2812b_load_rgba (&dev, leds);
  //printf (" color_loop_vardelay  tls_os_time_delay %d\n", t);
  tls_os_time_delay (t);
}

static void
white_temps ()
{ //-m18-SHOW A SAMPLE OF BLACK BODY RADIATION COLOR TEMPERATURES
  int N9 = (dev.led_numof / 9);
  for (int i = 0; i < dev.led_numof; i++)
    {
      if (i >= 0 && i < N9)
        {
          leds[i].color.r = 255; //-CANDLE - 1900
          leds[i].color.g = 147;
          leds[i].color.b = 41;
        }
      if (i >= N9 && i < N9 * 2)
        {
          leds[i].color.r = 255; //-40W TUNG - 2600
          leds[i].color.g = 197;
          leds[i].color.b = 143;
        }
      if (i >= N9 * 2 && i < N9 * 3)
        {
          leds[i].color.r = 255; //-100W TUNG - 2850
          leds[i].color.g = 214;
          leds[i].color.b = 170;
        }
      if (i >= N9 * 3 && i < N9 * 4)
        {
          leds[i].color.r = 255; //-HALOGEN - 3200
          leds[i].color.g = 241;
          leds[i].color.b = 224;
        }
      if (i >= N9 * 4 && i < N9 * 5)
        {
          leds[i].color.r = 255; //-CARBON ARC - 5200
          leds[i].color.g = 250;
          leds[i].color.b = 244;
        }
      if (i >= N9 * 5 && i < N9 * 6)
        {
          leds[i].color.r = 255; //-HIGH NOON SUN - 5400
          leds[i].color.g = 255;
          leds[i].color.b = 251;
        }
      if (i >= N9 * 6 && i < N9 * 7)
        {
          leds[i].color.r = 255; //-DIRECT SUN - 6000
          leds[i].color.g = 255;
          leds[i].color.b = 255;
        }
      if (i >= N9 * 7 && i < N9 * 8)
        {
          leds[i].color.r = 201; //-OVERCAST SKY - 7000
          leds[i].color.g = 226;
          leds[i].color.b = 255;
        }
      if (i >= N9 * 8 && i < dev.led_numof)
        {
          leds[i].color.r = 64; //-CLEAR BLUE SKY - 20000
          leds[i].color.g = 156;
          leds[i].color.b = 255;
        }
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (10);
}

static void
sin_bright_wave ()
{ //-m19-BRIGHTNESS SINE WAVE
  for (int i = 0; i < dev.led_numof; i++)
    {
      tcount = tcount + .1;
      if (tcount > 3.14)
        {
          tcount = 0.0;
        }
      ibright = (sin (tcount) * 255);
      CHSV (&leds[i].color, thishue, thissat, ibright);
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (u32_STEP);
    }
}

static void
pop_horizontal ()
{ //-m20-POP FROM LEFT TO RIGHT UP THE RING
  int ix = 0;
  if (bouncedirection == 0)
    {
      bouncedirection = 1;
      ix = idex;
    }
  else if (bouncedirection == 1)
    {
      bouncedirection = 0;
      ix = horizontal_index (idex);
      idex++;
      if (idex > TOP_INDEX)
        {
          idex = 0;
        }
    }
  for (int i = 0; i < dev.led_numof; i++)
    {
      if (i == ix)
        {
          CHSV (&leds[i].color, thishue, thissat, 255);
        }
      else
        {
          leds[i].color.r = 0;
          leds[i].color.g = 0;
          leds[i].color.b = 0;
        }
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
quad_bright_curve ()
{ //-m21-QUADRATIC BRIGHTNESS CURVER
  int ax;
  for (int x = 0; x < dev.led_numof; x++)
    {
      if (x <= TOP_INDEX)
        {
          ax = x;
        }
      else if (x > TOP_INDEX)
        {
          ax = dev.led_numof - x;
        }
      int a = 1;
      int b = 1;
      int c = 0;
      float iquad = -(ax * ax * a) + (ax * b) + c; //-ax2+bx+c
      float hquad = -(TOP_INDEX * TOP_INDEX * a) + (TOP_INDEX * b) + c;
      ibright = ((iquad / hquad) * 255);
      CHSV (&leds[x].color, thishue, thissat, ibright);
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
flame ()
{ //-m22-FLAMEISH EFFECT
  int idelay = random (0, 35);
  float hmin = 0.1;
  float hmax = 45.0;
  float hdif = hmax - hmin;
  int randtemp = random (0, 3);
  float hinc = (hdif / (float)TOP_INDEX) + randtemp;
  int ihue = hmin;
  for (int i = 0; i <= TOP_INDEX; i++)
    {
      ihue = ihue + hinc;
      CHSV (&leds[i].color, ihue, thissat, 255);
      int ih = horizontal_index (i);
      CHSV (&leds[ih].color, ihue, thissat, 255);
      leds[TOP_INDEX].color.r = 255;
      leds[TOP_INDEX].color.g = 255;
      leds[TOP_INDEX].color.b = 255;
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (idelay);
    }
}

static void
rainbow_vertical ()
{ //-m23-RAINBOW 'UP' THE LOOP
  idex++;
  if (idex > TOP_INDEX)
    {
      idex = 0;
    }
  ihue = ihue + thisstep;
  if (ihue > 255)
    {
      ihue = 0;
    }
  int idexA = idex;
  int idexB = horizontal_index (idexA);
  CHSV (&leds[idexA].color, ihue, thissat, 255);
  CHSV (&leds[idexB].color, ihue, thissat, 255);
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
pacman ()
{ //-m24-REALLY TERRIBLE PACMAN CHOMPING EFFECT
  int s = (dev.led_numof / 4);
  lcount++;
  if (lcount > 5)
    {
      lcount = 0;
    }
  if (lcount == 0)
    {
      for (int i = 0; i < dev.led_numof; i++)
        {
          set_color_led (i, 255, 255, 0);
        }
    }
  if (lcount == 1 || lcount == 5)
    {
      for (int i = 0; i < dev.led_numof; i++)
        {
          set_color_led (i, 255, 255, 0);
        }
      leds[s].color.r = 0;
      leds[s].color.g = 0;
      leds[s].color.b = 0;
    }
  if (lcount == 2 || lcount == 4)
    {
      for (int i = 0; i < dev.led_numof; i++)
        {
          set_color_led (i, 255, 255, 0);
        }
      leds[s - 1].color.r = 0;
      leds[s - 1].color.g = 0;
      leds[s - 1].color.b = 0;
      leds[s].color.r = 0;
      leds[s].color.g = 0;
      leds[s].color.b = 0;
      leds[s + 1].color.r = 0;
      leds[s + 1].color.g = 0;
      leds[s + 1].color.b = 0;
    }
  if (lcount == 3)
    {
      for (int i = 0; i < dev.led_numof; i++)
        {
          set_color_led (i, 255, 255, 0);
        }
      leds[s - 2].color.r = 0;
      leds[s - 2].color.g = 0;
      leds[s - 2].color.b = 0;
      leds[s - 1].color.r = 0;
      leds[s - 1].color.g = 0;
      leds[s - 1].color.b = 0;
      leds[s].color.r = 0;
      leds[s].color.g = 0;
      leds[s].color.b = 0;
      leds[s + 1].color.r = 0;
      leds[s + 1].color.g = 0;
      leds[s + 1].color.b = 0;
      leds[s + 2].color.r = 0;
      leds[s + 2].color.g = 0;
      leds[s + 2].color.b = 0;
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
random_color_pop ()
{ //-m25-RANDOM COLOR POP
  idex = random (0, dev.led_numof);
  ihue = random (0, 255);
  one_color_all (0, 0, 0);
  CHSV (&leds[idex].color, ihue, thissat, 255);
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
ems_lightsSTROBE ()
{ //-m26-EMERGENCY LIGHTS (STROBE LEFT/RIGHT)
  int thishue = 0;
  int thathue = (thishue + 160) % 255;
  for (int x = 0; x < 5; x++)
    {
      for (int i = 0; i < TOP_INDEX; i++)
        {
          CHSV (&leds[i].color, thishue, thissat, 255);
        }
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (u32_STEP);
      one_color_all (0, 0, 0);
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (u32_STEP);
    }
  for (int x = 0; x < 5; x++)
    {
      for (int i = TOP_INDEX; i < dev.led_numof; i++)
        {
          CHSV (&leds[i].color, thathue, thissat, 255);
        }
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (u32_STEP);
      one_color_all (0, 0, 0);
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (u32_STEP);
    }
}

static void
rgb_propeller ()
{ //-m27-RGB PROPELLER
  idex++;
  int ghue = (thishue + 80) % 255;
  int bhue = (thishue + 160) % 255;
  int N3 = (dev.led_numof / 3);
  int N6 = (dev.led_numof / 6);
  int N12 = (dev.led_numof / 12);
  for (int i = 0; i < N3; i++)
    {
      int j0 = (idex + i + dev.led_numof - N12) % dev.led_numof;
      int j1 = (j0 + N6) % dev.led_numof;
      int j2 = (j1 + N3) % dev.led_numof;
      CHSV (&leds[j0].color, thishue, thissat, 255);
      CHSV (&leds[j1].color, ghue, thissat, 255);
      CHSV (&leds[j2].color, bhue, thissat, 255);
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
kitt ()
{ //-m28-KNIGHT INDUSTIES 2000
  int rand = random (0, TOP_INDEX);
  for (int i = 0; i < rand; i++)
    {
      CHSV (&leds[TOP_INDEX + i].color, thishue, thissat, 255);
      CHSV (&leds[TOP_INDEX - i].color, thishue, thissat, 255);
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (thisdelay / rand);
    }
  for (int i = rand; i > 0; i--)
    {
      CHSV (&leds[TOP_INDEX + i].color, thishue, thissat, 0);
      CHSV (&leds[TOP_INDEX - i].color, thishue, thissat, 0);
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (thisdelay / rand);
    }
}

static void
matrix ()
{ //-m29-ONE LINE MATRIX
  int rand = random (0, 100);
  if (rand > 90)
    {
      CHSV (&leds[0].color, thishue, thissat, 255);
    }
  else
    {
      CHSV (&leds[0].color, thishue, thissat, 0);
    }
  copy_led_array ();
  for (int i = 1; i < dev.led_numof; i++)
    {
      leds[i].color.r = ledsX[i - 1][0];
      leds[i].color.g = ledsX[i - 1][1];
      leds[i].color.b = ledsX[i - 1][2];
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
strip_march_cw ()
{ //-m50-MARCH STRIP CW
  copy_led_array ();
  int iCW;
  for (int i = 0; i < dev.led_numof; i++)
    {
      iCW = adjacent_cw (i);
      leds[i].color.r = ledsX[iCW][0];
      leds[i].color.g = ledsX[iCW][1];
      leds[i].color.b = ledsX[iCW][2];
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
strip_march_ccw ()
{ //-m51-MARCH STRIP CCW
  copy_led_array ();
  int iCCW;
  for (int i = 0; i < dev.led_numof; i++)
    {
      iCCW = adjacent_ccw (i);
      leds[i].color.r = ledsX[iCCW][0];
      leds[i].color.g = ledsX[iCCW][1];
      leds[i].color.b = ledsX[iCCW][2];
    }
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
fill_rainbow (color_rgba_t pFirstLED[], int numToFill, uint8_t initialhue)
{
  uint8_t deltahue = 5;
  color_hsv_t hsv;
  hsv.h = initialhue;
  hsv.v = 255;
  hsv.s = 240;
  for (int i = 0; i < numToFill; i++)
    {
      CHSV (&pFirstLED[i].color, hsv.h, hsv.s, hsv.v);
      hsv.h += deltahue;
    }
}

static void
new_rainbow_loop ()
{ //-m88-RAINBOW FADE FROM FAST_SPI2
  ihue -= 1;
  fill_rainbow (leds, dev.led_numof, ihue);
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_STEP);
}

static void
demo_modeB ()
{
  int r = 10;
  one_color_all (0, 0, 0);
  ws2812b_load_rgba (&dev, leds);
  thisdelay = 35;
  for (int i = 0; i < r * 10; i++)
    {
      random_color_pop ();
    }
  for (int i = 0; i < r / 2; i++)
    {
      ems_lightsSTROBE ();
    }
  thisdelay = 50;
  for (int i = 0; i < r * 10; i++)
    {
      rgb_propeller ();
    }
  one_color_all (0, 0, 0);
  ws2812b_load_rgba (&dev, leds);
  thisdelay = 100;
  thishue = 0;
  for (int i = 0; i < r * 3; i++)
    {
      kitt ();
    }
  one_color_all (0, 0, 0);
  ws2812b_load_rgba (&dev, leds);
  thisdelay = 30;
  thishue = 95;
  for (int i = 0; i < r * 25; i++)
    {
      matrix ();
    }
  one_color_all (0, 0, 0);
  ws2812b_load_rgba (&dev, leds);
}

static void
demo_modeA ()
{
  int r = 10;
  thisdelay = 20;
  thisstep = 10;
  thishue = 0;
  thissat = 255;
  one_color_all (255, 255, 255);
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (1200);
  for (int i = 0; i < r * 25; i++)
    {
      rainbow_fade ();
    }
  for (int i = 0; i < r * 20; i++)
    {
      rainbow_loop ();
    }
  for (int i = 0; i < r * 20; i++)
    {
      random_burst ();
    }
  for (int i = 0; i < r * 12; i++)
    {
      color_bounce ();
    }
  thisdelay = 40;
  for (int i = 0; i < r * 12; i++)
    {
      color_bounceFADE ();
    }
  for (int i = 0; i < r * 6; i++)
    {
      ems_lightsONE ();
    }
  for (int i = 0; i < r * 5; i++)
    {
      ems_lightsALL ();
    }
  thishue = 160;
  thissat = 50;
  for (int i = 0; i < r * 40; i++)
    {
      flicker ();
    }
  one_color_all (0, 0, 0);
  ws2812b_load_rgba (&dev, leds);
  thisdelay = 15;
  thishue = 0;
  thissat = 255;
  for (int i = 0; i < r * 50; i++)
    {
      pulse_one_color_all ();
    }
  for (int i = 0; i < r * 40; i++)
    {
      pulse_one_color_all_rev ();
    }
  thisdelay = 60;
  thishue = 180;
  for (int i = 0; i < r * 5; i++)
    {
      fade_vertical ();
    }
  random_red ();
  thisdelay = 100;
  for (int i = 0; i < r * 5; i++)
    {
      rule30 ();
    }
  thisdelay = 40;
  for (int i = 0; i < r * 8; i++)
    {
      random_march ();
    }
  thisdelay = 80;
  for (int i = 0; i < r * 5; i++)
    {
      rwb_march ();
    }
  one_color_all (0, 0, 0);
  ;
  ws2812b_load_rgba (&dev, leds);
  thisdelay = 60;
  thishue = 95;
  for (int i = 0; i < r * 15; i++)
    {
      radiation ();
    }
  for (int i = 0; i < r * 15; i++)
    {
      color_loop_vardelay ();
    }
  for (int i = 0; i < r * 5; i++)
    {
      white_temps ();
    }
  thisdelay = 35;
  thishue = 180;
  for (int i = 0; i < r; i++)
    {
      sin_bright_wave ();
    }
  thisdelay = 100;
  thishue = 0;
  for (int i = 0; i < r * 5; i++)
    {
      pop_horizontal ();
    }
  thisdelay = 100;
  thishue = 180;
  for (int i = 0; i < r * 4; i++)
    {
      quad_bright_curve ();
    }
  one_color_all (0, 0, 0);
  ws2812b_load_rgba (&dev, leds);
  for (int i = 0; i < r * 3; i++)
    {
      flame ();
    }
  thisdelay = 50;
  for (int i = 0; i < r * 10; i++)
    {
      pacman ();
    }
  thisdelay = 50;
  thisstep = 15;
  for (int i = 0; i < r * 12; i++)
    {
      rainbow_vertical ();
    }
  thisdelay = 100;
  for (int i = 0; i < r * 3; i++)
    {
      strip_march_ccw ();
    }
  for (int i = 0; i < r * 3; i++)
    {
      strip_march_cw ();
    }
  demo_modeB ();
  thisdelay = 5;
  for (int i = 0; i < r * 120; i++)
    {
      new_rainbow_loop ();
    }
  one_color_all (255, 0, 0);
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (1200);
  one_color_all (0, 255, 0);
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (1200);
  one_color_all (0, 0, 255);
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (1200);
  one_color_all (255, 255, 0);
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (1200);
  one_color_all (0, 255, 255);
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (1200);
  one_color_all (255, 0, 255);
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (1200);
}

//-----------------------------плавное заполнение
//цветом-----------------------------------------
static void
colorWipe (u8 red, u8 green, u8 blue, int SpeedDelay)
{
  for (uint16_t i = 0; i < dev.led_numof; i++)
    {
      setPixel (i, red, green, blue);
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (u32_DIM);
    }
}

//-----------------------------------бегающие
//светодиоды-----------------------------------
static void
CylonBounce (u8 red, u8 green, u8 blue, int EyeSize, int SpeedDelay,
             int ReturnDelay)
{

  for (int i = 0; i < dev.led_numof - EyeSize - 2; i++)
    {
      setAll (0, 0, 0);
      setPixel (i, red / 10, green / 10, blue / 10);
      for (int j = 1; j <= EyeSize; j++)
        {
          setPixel (i + j, red, green, blue);
        }
      setPixel (i + EyeSize + 1, red / 10, green / 10, blue / 10);
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (u32_DIM);
    }

  tls_os_time_delay (ReturnDelay);

  for (int i = dev.led_numof - EyeSize - 2; i > 0; i--)
    {
      setAll (0, 0, 0);
      setPixel (i, red / 10, green / 10, blue / 10);
      for (int j = 1; j <= EyeSize; j++)
        {
          setPixel (i + j, red, green, blue);
        }
      setPixel (i + EyeSize + 1, red / 10, green / 10, blue / 10);
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (u32_DIM);
    }

  tls_os_time_delay (ReturnDelay);
}

static void
setPixelHeatColor (int Pixel, u8 temperature)
{
  // Scale 'heat' down from 0-255 to 0-191
  u8 t192 = round ((temperature / 255.0) * 191);

  // calculate ramp up from
  u8 heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2;            // scale up to 0..252

  // figure out which third of the spectrum we're in:
  if (t192 > 0x80)
    { // hottest
      setPixel (Pixel, 255, 255, heatramp);
    }
  else if (t192 > 0x40)
    { // middle
      setPixel (Pixel, 255, heatramp, 0);
    }
  else
    { // coolest
      setPixel (Pixel, heatramp, 0, 0);
    }
}

//---------------------------------линейный
//огонь-------------------------------------
static void
Fire (int Cooling, int Sparking, int SpeedDelay)
{
  static u8 heat[MAX_WS2812B_LED_NUMOF];
  int cooldown;

  // Step 1.  Cool down every cell a little
  for (int i = 0; i < dev.led_numof; i++)
    {
      cooldown = random (0, ((Cooling * 10) / dev.led_numof) + 2);

      if (cooldown > heat[i])
        {
          heat[i] = 0;
        }
      else
        {
          heat[i] = heat[i] - cooldown;
        }
    }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for (int k = dev.led_numof - 1; k >= 2; k--)
    {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
    }

  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if (random (0, 255) < Sparking)
    {
      int y = random (0, 7);
      heat[y] = heat[y] + random (160, 255);
      // heat[y] = random(160,255);
    }

  // Step 4.  Convert heat to LED colors
  for (int j = 0; j < dev.led_numof; j++)
    {
      setPixelHeatColor (j, heat[j]);
    }

  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_DIM);
}

//-------------------------------newKITT---------------------------------------
static void
CenterToOutside (u8 red, u8 green, u8 blue, int EyeSize, int SpeedDelay,
                 int ReturnDelay)
{
  for (int i = ((dev.led_numof - EyeSize) / 2); i >= 0; i--)
    {
      setAll (0, 0, 0);

      setPixel (i, red / 10, green / 10, blue / 10);
      for (int j = 1; j <= EyeSize; j++)
        {
          setPixel (i + j, red, green, blue);
        }
      setPixel (i + EyeSize + 1, red / 10, green / 10, blue / 10);

      setPixel (dev.led_numof - i, red / 10, green / 10, blue / 10);
      for (int j = 1; j <= EyeSize; j++)
        {
          setPixel (dev.led_numof - i - j, red, green, blue);
        }
      setPixel (dev.led_numof - i - EyeSize - 1, red / 10, green / 10,
                blue / 10);

      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (u32_DIM);
    }
  tls_os_time_delay (ReturnDelay);
}

static void
OutsideToCenter (u8 red, u8 green, u8 blue, int EyeSize, int SpeedDelay,
                 int ReturnDelay)
{
  for (int i = 0; i <= ((dev.led_numof - EyeSize) / 2); i++)
    {
      setAll (0, 0, 0);

      setPixel (i, red / 10, green / 10, blue / 10);
      for (int j = 1; j <= EyeSize; j++)
        {
          setPixel (i + j, red, green, blue);
        }
      setPixel (i + EyeSize + 1, red / 10, green / 10, blue / 10);

      setPixel (dev.led_numof - i, red / 10, green / 10, blue / 10);
      for (int j = 1; j <= EyeSize; j++)
        {
          setPixel (dev.led_numof - i - j, red, green, blue);
        }
      setPixel (dev.led_numof - i - EyeSize - 1, red / 10, green / 10,
                blue / 10);

      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (u32_DIM);
    }
  tls_os_time_delay (ReturnDelay);
}

static void
LeftToRight (u8 red, u8 green, u8 blue, int EyeSize, int SpeedDelay,
             int ReturnDelay)
{
  for (int i = 0; i < dev.led_numof - EyeSize - 2; i++)
    {
      setAll (0, 0, 0);
      setPixel (i, red / 10, green / 10, blue / 10);
      for (int j = 1; j <= EyeSize; j++)
        {
          setPixel (i + j, red, green, blue);
        }
      setPixel (i + EyeSize + 1, red / 10, green / 10, blue / 10);
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (u32_DIM);
    }
  tls_os_time_delay (ReturnDelay);
}

static void
RightToLeft (u8 red, u8 green, u8 blue, int EyeSize, int SpeedDelay,
             int ReturnDelay)
{
  for (int i = dev.led_numof - EyeSize - 2; i > 0; i--)
    {
      setAll (0, 0, 0);
      setPixel (i, red / 10, green / 10, blue / 10);
      for (int j = 1; j <= EyeSize; j++)
        {
          setPixel (i + j, red, green, blue);
        }
      setPixel (i + EyeSize + 1, red / 10, green / 10, blue / 10);
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (u32_DIM);
    }
  tls_os_time_delay (ReturnDelay);
}

static void
NewKITT (u8 red, u8 green, u8 blue, int EyeSize, int SpeedDelay,
         int ReturnDelay)
{
  RightToLeft (red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  LeftToRight (red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  OutsideToCenter (red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  CenterToOutside (red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  LeftToRight (red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  RightToLeft (red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  OutsideToCenter (red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  CenterToOutside (red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
}

//-------------------------------newKITT---------------------------------------
static u8 *
Wheel (u8 WheelPos)
{
  static u8 c[3];

  if (WheelPos < 85)
    {
      c[0] = WheelPos * 3;
      c[1] = 255 - WheelPos * 3;
      c[2] = 0;
    }
  else if (WheelPos < 170)
    {
      WheelPos -= 85;
      c[0] = 255 - WheelPos * 3;
      c[1] = 0;
      c[2] = WheelPos * 3;
    }
  else
    {
      WheelPos -= 170;
      c[0] = 0;
      c[1] = WheelPos * 3;
      c[2] = 255 - WheelPos * 3;
    }

  return c;
}

static void
rainbowCycle (int SpeedDelay)
{
  u8 *c;
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++)
    { // 5 cycles of all colors on wheel
      for (i = 0; i < dev.led_numof; i++)
        {
          c = Wheel (((i * 256 / dev.led_numof) + j) & 255);
          setPixel (i, *c, *(c + 1), *(c + 2));
        }
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (u32_DIM);
    }
}

//-------------------------------TwinkleRandom---------------------------------------
static void
TwinkleRandom (int Count, int SpeedDelay, bool OnlyOne)
{
  setAll (0, 0, 0);

  for (int i = 0; i < Count; i++)
    {
      setPixel (random (0, dev.led_numof), random (0, 255), random (0, 255),
                random (0, 255));
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (u32_DIM);
      if (OnlyOne)
        {
          setAll (0, 0, 0);
        }
    }

  tls_os_time_delay (u32_DIM);
}

//-------------------------------RunningLights---------------------------------------
static void
RunningLights (u8 red, u8 green, u8 blue, int WaveDelay)
{
  int Position = 0;

  for (int i = 0; i < dev.led_numof * 2; i++)
    {
      Position++; // = 0; //Position + Rate;
      for (int i = 0; i < dev.led_numof; i++)
        {
          // sine wave, 3 offset waves make a rainbow!
          // float level = sin(i+Position) * 127 + 128;
          // setPixel(i,level,0,0);
          // float level = sin(i+Position) * 127 + 128;
          setPixel (i, ((sin (i + Position) * 127 + 128) / 255) * red,
                    ((sin (i + Position) * 127 + 128) / 255) * green,
                    ((sin (i + Position) * 127 + 128) / 255) * blue);
        }

      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (WaveDelay);
    }
}

//-------------------------------Sparkle---------------------------------------
static void
Sparkle (u8 red, u8 green, u8 blue, int SpeedDelay)
{
  int Pixel = random (0, dev.led_numof);
  setPixel (Pixel, red, green, blue);
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_DIM);
  setPixel (Pixel, 0, 0, 0);
}

//-------------------------------SnowSparkle---------------------------------------
static void
SnowSparkle (u8 red, u8 green, u8 blue, int SparkleDelay, int SpeedDelay)
{
  setAll (red, green, blue);

  int Pixel = random (0, dev.led_numof);
  setPixel (Pixel, 0xff, 0xff, 0xff);
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (SparkleDelay);
  setPixel (Pixel, red, green, blue);
  ws2812b_load_rgba (&dev, leds);
  tls_os_time_delay (u32_DIM);
}

//-------------------------------theaterChase---------------------------------------
void
theaterChase (u8 red, u8 green, u8 blue, int SpeedDelay)
{
  for (int j = 0; j < 10; j++)
    { // do 10 cycles of chasing
      for (int q = 0; q < 3; q++)
        {
          for (int i = 0; i < dev.led_numof; i = i + 3)
            {
              setPixel (i + q, red, green, blue); // turn every third pixel on
            }
          ws2812b_load_rgba (&dev, leds);
          tls_os_time_delay (u32_DIM);
          for (int i = 0; i < dev.led_numof; i = i + 3)
            {
              setPixel (i + q, 0, 0, 0); // turn every third pixel off
            }
        }
    }
}

//-------------------------------theaterChaseRainbow---------------------------------------
static void
theaterChaseRainbow (int SpeedDelay)
{
  u8 *c;

  for (int j = 0; j < 256; j++)
    { // cycle all 256 colors in the wheel
      for (int q = 0; q < 3; q++)
        {
          for (int i = 0; i < dev.led_numof; i = i + 3)
            {
              c = Wheel ((i + j) % 255);
              setPixel (i + q, *c, *(c + 1),
                        *(c + 2)); // turn every third pixel on
            }
          ws2812b_load_rgba (&dev, leds);
          tls_os_time_delay (u32_DIM);
          for (int i = 0; i < dev.led_numof; i = i + 3)
            {
              setPixel (i + q, 0, 0, 0); // turn every third pixel off
            }
        }
    }
}

//-------------------------------Strobe---------------------------------------
static void
Strobe (u8 red, u8 green, u8 blue, int StrobeCount, int FlashDelay,
        int EndPause)
{
  for (int j = 0; j < StrobeCount; j++)
    {
      setAll (red, green, blue);
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (FlashDelay);
      setAll (0, 0, 0);
      ws2812b_load_rgba (&dev, leds);
      tls_os_time_delay (FlashDelay);
    }

  tls_os_time_delay (EndPause);
}

//-------------------------------BouncingBalls---------------------------------------
static void
BouncingBalls (u8 red, u8 green, u8 blue, int BallCount)
{
  float Gravity = -9.81;
  int StartHeight = 1;

  float Height[BallCount];
  float ImpactVelocityStart = sqrt (-2 * Gravity * StartHeight);
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int Position[BallCount];
  long ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];

  for (int i = 0; i < BallCount; i++)
    {
      ClockTimeSinceLastBounce[i] = tls_os_get_time ();
      Height[i] = StartHeight;
      Position[i] = 0;
      ImpactVelocity[i] = ImpactVelocityStart;
      TimeSinceLastBounce[i] = 0;
      Dampening[i] = 0.90 - (float)i / pow (BallCount, 2);
    }

  while (true)
    {
      for (int i = 0; i < BallCount; i++)
        {
          TimeSinceLastBounce[i]
              = tls_os_get_time () - ClockTimeSinceLastBounce[i];
          Height[i] = 0.5 * Gravity * pow (TimeSinceLastBounce[i] / 1000, 2.0)
                      + ImpactVelocity[i] * TimeSinceLastBounce[i] / 1000;

          if (Height[i] < 0)
            {
              Height[i] = 0;
              ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
              ClockTimeSinceLastBounce[i] = tls_os_get_time ();

              if (ImpactVelocity[i] < 0.01)
                {
                  ImpactVelocity[i] = ImpactVelocityStart;
                }
            }
          Position[i] = round (Height[i] * (dev.led_numof - 1) / StartHeight);
        }

      for (int i = 0; i < BallCount; i++)
        {
          setPixel (Position[i], red, green, blue);
        }
      ws2812b_load_rgba (&dev, leds);
      setAll (0, 0, 0);
    }
}

//-------------------------------BouncingColoredBalls---------------------------------------
static void
BouncingColoredBalls (int BallCount, u8 colors[][3])
{
  float Gravity = -9.81;
  int StartHeight = 1;

  float Height[BallCount];
  float ImpactVelocityStart = sqrt (-2 * Gravity * StartHeight);
  float ImpactVelocity[BallCount];
  float TimeSinceLastBounce[BallCount];
  int Position[BallCount];
  long ClockTimeSinceLastBounce[BallCount];
  float Dampening[BallCount];

  for (int i = 0; i < BallCount; i++)
    {
      ClockTimeSinceLastBounce[i] = tls_os_get_time ();
      Height[i] = StartHeight;
      Position[i] = 0;
      ImpactVelocity[i] = ImpactVelocityStart;
      TimeSinceLastBounce[i] = 0;
      Dampening[i] = 0.90 - (float)i / pow (BallCount, 2);
    }

  while (true)
    {
      for (int i = 0; i < BallCount; i++)
        {
          TimeSinceLastBounce[i]
              = tls_os_get_time () - ClockTimeSinceLastBounce[i];
          Height[i] = 0.5 * Gravity * pow (TimeSinceLastBounce[i] / 1000, 2.0)
                      + ImpactVelocity[i] * TimeSinceLastBounce[i] / 1000;

          if (Height[i] < 0)
            {
              Height[i] = 0;
              ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
              ClockTimeSinceLastBounce[i] = tls_os_get_time ();

              if (ImpactVelocity[i] < 0.01)
                {
                  ImpactVelocity[i] = ImpactVelocityStart;
                }
            }
          Position[i] = round (Height[i] * (dev.led_numof - 1) / StartHeight);
        }

      for (int i = 0; i < BallCount; i++)
        {
          setPixel (Position[i], colors[i][0], colors[i][1], colors[i][2]);
        }
      ws2812b_load_rgba (&dev, leds);
      setAll (0, 0, 0);
    }
}

//--------------------------

static void
change_mode (int newmode)
{
  thissat = 255;
  switch (newmode)
    {
    case 0:
      one_color_all (0, 0, 0);
      ws2812b_load_rgba (&dev, leds);
      break; //---ALL OFF
    case 1:
      one_color_all (255, 255, 255);
      ws2812b_load_rgba (&dev, leds);
      break; //---ALL ON
    case 2:
      thisdelay = 20;
      break; //---STRIP RAINBOW FADE
    case 3:
      thisdelay = 20;
      thisstep = 10;
      break; //---RAINBOW LOOP
    case 4:
      thisdelay = 20;
      break; //---RANDOM BURST
    case 5:
      thisdelay = 20;
      thishue = 0;
      break; //---CYLON v1
    case 6:
      thisdelay = 40;
      thishue = 0;
      break; //---CYLON v2
    case 7:
      thisdelay = 40;
      thishue = 0;
      break; //---POLICE LIGHTS SINGLE
    case 8:
      thisdelay = 40;
      thishue = 0;
      break; //---POLICE LIGHTS SOLID
    case 9:
      thishue = 160;
      thissat = 50;
      break; //---STRIP FLICKER
    case 10:
      thisdelay = 15;
      thishue = 0;
      break; //---PULSE COLOR BRIGHTNESS
    case 11:
      thisdelay = 15;
      thishue = 0;
      break; //---PULSE COLOR SATURATION
    case 12:
      thisdelay = 60;
      thishue = 180;
      break; //---VERTICAL SOMETHING
    case 13:
      thisdelay = 100;
      break; //---CELL AUTO - RULE 30 (RED)
    case 14:
      thisdelay = 40;
      break; //---MARCH RANDOM COLORS
    case 15:
      thisdelay = 80;
      break; //---MARCH RWB COLORS
    case 16:
      thisdelay = 60;
      thishue = 95;
      break; //---RADIATION SYMBOL
    //---PLACEHOLDER FOR COLOR LOOP VAR DELAY VARS
    case 19:
      thisdelay = 35;
      thishue = 180;
      break; //---SIN WAVE BRIGHTNESS
    case 20:
      thisdelay = 100;
      thishue = 0;
      break; //---POP LEFT/RIGHT
    case 21:
      thisdelay = 100;
      thishue = 180;
      break; //---QUADRATIC BRIGHTNESS CURVE
    //---PLACEHOLDER FOR FLAME VARS
    case 23:
      thisdelay = 50;
      thisstep = 15;
      break; //---VERITCAL RAINBOW
    case 24:
      thisdelay = 50;
      break; //---PACMAN
    case 25:
      thisdelay = 35;
      break; //---RANDOM COLOR POP
    case 26:
      thisdelay = 25;
      thishue = 0;
      break; //---EMERGECNY STROBE
    case 27:
      thisdelay = 25;
      thishue = 0;
      break; //---RGB PROPELLER
    case 28:
      thisdelay = 100;
      thishue = 0;
      break; //---KITT
    case 29:
      thisdelay = 50;
      thishue = 95;
      break; //---MATRIX RAIN
    case 30:
      thisdelay = 5;
      break; //---NEW RAINBOW LOOP
    case 31:
      thisdelay = 100;
      break; //---MARCH STRIP NOW CCW
    case 32:
      thisdelay = 100;
      break; //---MARCH STRIP NOW CCW
    case 33:
      thisdelay = 50;
      break; // colorWipe
    case 34:
      thisdelay = 50;
      break; // CylonBounce
    case 35:
      thisdelay = 15;
      break; // Fire
    case 36:
      thisdelay = 50;
      break; // NewKITT
    case 37:
      thisdelay = 20;
      break; // rainbowCycle
    case 38:
      thisdelay = 10;
      break; // rainbowTwinkle
    case 39:
      thisdelay = 50;
      break; // RunningLights
    case 40:
      thisdelay = 0;
      break; // Sparkle
    case 41:
      thisdelay = 20;
      break; // SnowSparkle
    case 42:
      thisdelay = 50;
      break; // theaterChase
    case 43:
      thisdelay = 50;
      break; // theaterChaseRainbow
    case 44:
      thisdelay = 100;
      break; // Strobe

    case 101:
      one_color_all (255, 0, 0);
      ws2812b_load_rgba (&dev, leds);
      break; //---ALL RED
    case 102:
      one_color_all (0, 255, 0);
      ws2812b_load_rgba (&dev, leds);
      break; //---ALL GREEN
    case 103:
      one_color_all (0, 0, 255);
      ws2812b_load_rgba (&dev, leds);
      break; //---ALL BLUE
    case 104:
      one_color_all (255, 255, 0);
      ws2812b_load_rgba (&dev, leds);
      break; //---ALL COLOR X
    case 105:
      one_color_all (0, 255, 255);
      ws2812b_load_rgba (&dev, leds);
      break; //---ALL COLOR Y
    case 106:
      one_color_all (255, 0, 255);
      ws2812b_load_rgba (&dev, leds);
      break; //---ALL COLOR Z
    }
  bouncedirection = 0;
  one_color_all (0, 0, 0);
  ledMode = newmode;
}

void
el_loop (int new_mode)
{

  if (new_mode > 0 && ledMode != new_mode)
    { // если что то прислали
      ledMode = new_mode;
      change_mode (ledMode); // меняем режим через change_mode (там для каждого
                             // режима стоят цвета и задержки)
    }
  switch (ledMode)
    {
    case 999:
      break; // пазуа
    case 2:
      rainbow_fade ();
      break; // плавная смена цветов всей ленты
    case 3:
      rainbow_loop ();
      break; // крутящаяся радуга
    case 4:
      random_burst ();
      break; // случайная смена цветов
    case 5:
      color_bounce ();
      break; // бегающий светодиод
    case 6:
      color_bounceFADE ();
      break; // бегающий паровозик светодиодов
    case 7:
      ems_lightsONE ();
      break; // вращаются красный и синий
    case 8:
      ems_lightsALL ();
      break; // вращается половина красных и половина синих
    case 9:
      flicker ();
      break; // случайный стробоскоп
    case 10:
      pulse_one_color_all ();
      break; // пульсация одним цветом
    case 11:
      pulse_one_color_all_rev ();
      break; // пульсация со сменой цветов
    case 12:
      fade_vertical ();
      break; // плавная смена яркости по вертикали (для кольца)
    case 13:
      rule30 ();
      break; // безумие красных светодиодов
    case 14:
      random_march ();
      break; // безумие случайных цветов
    case 15:
      rwb_march ();
      break; // белый синий красный бегут по кругу (ПАТРИОТИЗМ!)
    case 16:
      radiation ();
      break; // пульсирует значок радиации
    case 17:
      color_loop_vardelay ();
      break; // красный светодиод бегает по кругу
    case 18:
      white_temps ();
      break; // бело синий градиент (?)
    case 19:
      sin_bright_wave ();
      break; // тоже хрень какая то
    case 20:
      pop_horizontal ();
      break; // красные вспышки спускаются вниз
    case 21:
      quad_bright_curve ();
      break; // полумесяц
    case 22:
      flame ();
      break; // эффект пламени
    case 23:
      rainbow_vertical ();
      break; // радуга в вертикаьной плоскости (кольцо)
    case 24:
      pacman ();
      break; // пакман
    case 25:
      random_color_pop ();
      break; // безумие случайных вспышек
    case 26:
      ems_lightsSTROBE ();
      break; // полицейская мигалка
    case 27:
      rgb_propeller ();
      break; // RGB пропеллер
    case 28:
      kitt ();
      break; // случайные вспышки красного в вертикаьной плоскости
    case 29:
      matrix ();
      break; // зелёненькие бегают по кругу случайно
    case 30:
      new_rainbow_loop ();
      break; // крутая плавная вращающаяся радуга
    case 31:
      strip_march_ccw ();
      break; // чёт сломалось
    case 32:
      strip_march_cw ();
      break; // чёт сломалось
    case 33:
      colorWipe (0x00, 0xff, 0x00, thisdelay);
      colorWipe (0x00, 0x00, 0x00, thisdelay);
      break; // плавное заполнение цветом
    case 34:
      CylonBounce (0xff, 0, 0, 4, 10, thisdelay);
      break; // бегающие светодиоды
    case 35:
      Fire (55, 120, thisdelay);
      break; // линейный огонь
    case 36:
      NewKITT (0xff, 0, 0, 8, 10, thisdelay);
      break; // беготня секторов круга (не работает)
    case 37:
      rainbowCycle (thisdelay);
      break; // очень плавная вращающаяся радуга
    case 38:
      TwinkleRandom (20, thisdelay, 1);
      break; // случайные разноцветные включения (1 - танцуют все, 0 -
             // случайный 1 диод)
    case 39:
      RunningLights (0xff, 0xff, 0x00, thisdelay);
      break; // бегущие огни
    case 40:
      Sparkle (0xff, 0xff, 0xff, thisdelay);
      break; // случайные вспышки белого цвета
    case 41:
      SnowSparkle (0x10, 0x10, 0x10, thisdelay, random (100, 1000));
      break; // случайные вспышки белого цвета на белом фоне
    case 42:
      theaterChase (0xff, 0, 0, thisdelay);
      break; // бегущие каждые 3 (ЧИСЛО СВЕТОДИОДОВ ДОЛЖНО БЫТЬ НЕЧЁТНОЕ)
    case 43:
      theaterChaseRainbow (thisdelay);
      break; // бегущие каждые 3 радуга (ЧИСЛО СВЕТОДИОДОВ ДОЛЖНО БЫТЬ КРАТНО
             // 3)
    case 44:
      Strobe (0xff, 0xff, 0xff, 10, thisdelay, 1000);
      break; // стробоскоп

    case 45:
      BouncingBalls (0xff, 0, 0, 3);
      break; // прыгающие мячики
    case 46:
      BouncingColoredBalls (3, ballColors);
      break; // прыгающие мячики цветные

    case 47:
      ef_1 ();
      break; //
    case 48:
      ef_2 ();
      break; //

    case 888:
      demo_modeA ();
      break; // длинное демо
    case 889:
      demo_modeB ();
      break; // короткое демо
    }
  tls_os_time_delay (1);
}
