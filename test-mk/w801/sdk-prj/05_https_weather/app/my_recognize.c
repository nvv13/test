
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"

#include "my_recognize.h"

static int i_temperature_c = MY_RECOGNIZE_NO_VALUE;
static int i_sign = 1;
static int i_temperature_mantissa_c = 0;
static struct tm t_last_query;

struct tm
my_recognize_ret_t_last_query (void)
{
  return t_last_query;
}
int
my_recognize_ret_cur_temperature (void)
{
  return i_temperature_c;
}
int
my_recognize_ret_cur_temperature_sign (void)
{
  return i_sign;
}
int
my_recognize_ret_cur_temperature_mantissa (void)
{
  return i_temperature_mantissa_c;
}

enum
{
  RG_START_FIND = 0,
  RG_FIND_PRE2,
  RG_FIND_PRE3,
  RG_FIND_END
};

static u8 u8_status_find = RG_START_FIND;

char *c_PRE1 = "id=\"weather-now-number\">";
int i_POS_PRE1 = 0;

char *c_PRE2 = "<";
int i_POS_PRE2 = 0;

char buf_temperatura[10];
u8 u8_buf_pos_temperatura = 0;

void
my_recognize_http_reset (void)
{
  if (u8_status_find != RG_FIND_END)
    {
      i_temperature_c = MY_RECOGNIZE_NO_VALUE;
      i_sign = 1;
      i_temperature_mantissa_c = 0;
    }
  //
  u8_status_find = RG_START_FIND;
  i_POS_PRE1 = 0;
  i_POS_PRE2 = 0;
  u8_buf_pos_temperatura = 0;
}

void
my_recognize_http_error (void)
{
  my_recognize_http_reset ();
  i_temperature_c = MY_RECOGNIZE_NO_VALUE;
  i_sign = 1;
  i_temperature_mantissa_c = 0;
}

// <div id="weather-now-number">-29<span>°</span></div>
void
my_recognize_http (const char *recvbuf, int i_len)
{
  for (int iInd = 0; iInd < i_len; iInd++)
    {
      char ch = *(recvbuf + iInd);
      switch (u8_status_find)
        {
        case RG_START_FIND:
          {
            if (ch == c_PRE1[i_POS_PRE1]) // id="weather-now-number">
              i_POS_PRE1++;
            else
              {
                if (i_POS_PRE1 < strlen (c_PRE1))
                  i_POS_PRE1 = 0;
                else
                  {
                    if (u8_buf_pos_temperatura < 9)
                      buf_temperatura[u8_buf_pos_temperatura++] = ch;
                    u8_status_find = RG_FIND_PRE2;
                  }
              }
          };
          break;

        case RG_FIND_PRE2:
          {
            if (ch
                == c_PRE2[i_POS_PRE2]) // далее идёт число со знаком +
                                       // температура -29<span>°</span></div>
              i_POS_PRE2++;
            else
              {
                if (i_POS_PRE2 < strlen (c_PRE2))
                  {
                    if (u8_buf_pos_temperatura < 9)
                      buf_temperatura[u8_buf_pos_temperatura++] = ch;
                    i_POS_PRE2 = 0;
                  }
                else
                  {
                    // if (u8_buf_pos_temperatura < 9)
                    //  buf_temperatura[u8_buf_pos_temperatura++] = ch;
                    u8_status_find = RG_FIND_PRE3;
                  }
              }
          };
          break;

        case RG_FIND_PRE3:
          {
            bool l_znak = false;
            if (buf_temperatura[0] == '-')
              {
                i_sign = 0;
                l_znak = true;
              }
            else
              i_sign = 1;
            if (buf_temperatura[0] == '+')
              {
                i_sign = 1;
                l_znak = true;
              }
            if (u8_buf_pos_temperatura < 10)
              buf_temperatura[u8_buf_pos_temperatura++] = 0x00;
            if (l_znak)
              i_temperature_c = atoi (buf_temperatura + 1);
            else
              i_temperature_c = atoi (buf_temperatura);
            printf ("%s \r\n", buf_temperatura);
            u8_status_find = RG_FIND_END;
            tls_get_rtc (&t_last_query);
            //          i_temperature_mantissa_c = ch - 0x30;
          };
          break;
        }
    }
}