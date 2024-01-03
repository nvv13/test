
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

static char *c_PRE1 = "<meta name=\"description\" content=";
static int i_POS_PRE1 = 0;

static char *c_PRE2 = "/>";
static int i_POS_PRE2 = 0;

#define BUF_SIZE 512
static char buf_temperatura[BUF_SIZE];
static u16 u8_buf_pos_temperatura = 0;

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

//     <meta name="description" content="В Архангельске сегодня ожидается
//     -19..-15 °C, преимущественно без осадков, туман, легкий ветер. Завтра:
//     -19..-22 °C, без осадков, туман, легкий ветер. РП5" />
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
            if (ch == c_PRE1[i_POS_PRE1]) // <meta name="description" content="
              i_POS_PRE1++;
            else
              {
                if (i_POS_PRE1 < strlen (c_PRE1))
                  i_POS_PRE1 = 0;
                else
                  {
                    if (u8_buf_pos_temperatura < BUF_SIZE - 1)
                      buf_temperatura[u8_buf_pos_temperatura++] = ch;
                    u8_status_find = RG_FIND_PRE2;
                  }
              }
          };
          break;

        case RG_FIND_PRE2:
          {
            if (ch == c_PRE2[i_POS_PRE2]) // />
              i_POS_PRE2++;
            else
              {
                if (i_POS_PRE2 < strlen (c_PRE2))
                  {
                    if (u8_buf_pos_temperatura < BUF_SIZE - 1)
                      buf_temperatura[u8_buf_pos_temperatura++] = ch;
                    i_POS_PRE2 = 0;
                  }
                else
                  {
                    u8_status_find = RG_FIND_PRE3;
                  }
              }
          };
          break;

        case RG_FIND_PRE3:
          {
            bool l_znak = true;
            i_sign = 1;
            if (u8_buf_pos_temperatura < BUF_SIZE)
              buf_temperatura[u8_buf_pos_temperatura++] = 0x00;

            // "В Архангельске сегодня ожидается -17..-15 °C, преимущественно
            // без осадков, туман, легкий ветер. Завтра: -19..-22 °C, без
            // осадков, туман, легкий ветер. РП5"
            char buf[10];
            u8 u8_pos = 0;
            for (int iI = 0; iI < u8_buf_pos_temperatura; iI++)
              {
                char ch = buf_temperatura[iI];
                if (iI > 0 && ch >= '0' && ch<= '9')
                  {
                    buf[u8_pos++] = ch;
                    if (l_znak)
                      {
                        l_znak = false;
                        if (buf_temperatura[iI - 1] == '-')
                          {
                            i_sign = 0;
                          }
                      }
                  }
                else
                  {
                    if (!l_znak)
                      {
                        buf[u8_pos++] = 0x00;
                        i_temperature_c = atoi (buf);
                        break;
                      }
                  }
              }

            printf ("load: %s, %d\r\n", buf_temperatura, i_temperature_c);
            u8_status_find = RG_FIND_END;
            tls_get_rtc (&t_last_query);
            //          i_temperature_mantissa_c = ch - 0x30;
          };
          break;
        }
    }
}