
#include <string.h>
#include <stdlib.h>

#include "wm_type_def.h"

#include "my_recognize.h"

static int i_temperature_c=MY_RECOGNIZE_NO_VALUE;
static int i_sign=1;
static int i_temperature_mantissa_c=0;
static struct tm t_last_query;

struct tm my_recognize_ret_t_last_query(void)
{
return t_last_query;
}
int my_recognize_ret_cur_temperature(void)
{
return i_temperature_c;
}
int my_recognize_ret_cur_temperature_sign(void)
{
return i_sign;
}
int my_recognize_ret_cur_temperature_mantissa(void)
{
return i_temperature_mantissa_c;
}

enum 
{
    RG_START_FIND = 0,
    RG_FIND_PRE2,
    RG_FIND_PRE3,
    RG_FIND_PRE4,
    RG_FIND_PRE5,
    RG_FIND_PRE6,
    RG_FIND_END
};

static u8 u8_status_find=RG_START_FIND;

char* c_PRE1="weather-value";
int   i_POS_PRE1=0;

char* c_PRE2="unit_temperature_c";
int   i_POS_PRE2=0;

char* c_PRE3="sign\">";
int   i_POS_PRE3=0;

char* c_PRE4="</span>";
int   i_POS_PRE4=0;

char buf_temperatura[10];
u8 u8_buf_pos_temperatura=0;

char* c_PRE5="<";
int   i_POS_PRE5=0;

char* c_PRE6="temperature_f";
int   i_POS_PRE6=0;

char* c_PRE7="lower\">,";
int   i_POS_PRE7=0;

void my_recognize_http_reset(void)
{
if(u8_status_find!=RG_FIND_END)my_recognize_http_error();
//
u8_status_find=RG_START_FIND;
i_POS_PRE1=0;
i_POS_PRE2=0;
i_POS_PRE3=0;
i_POS_PRE4=0;
i_POS_PRE5=0;
i_POS_PRE6=0;
i_POS_PRE7=0;
u8_buf_pos_temperatura=0;
}

void my_recognize_http_error(void)
{
my_recognize_http_reset();
i_temperature_c=MY_RECOGNIZE_NO_VALUE;
i_sign=1;
i_temperature_mantissa_c=0;
}

void my_recognize_http(const char * recvbuf, int i_len)
{
for(int iInd=0;iInd<i_len;iInd++)
 {
 char ch=*(recvbuf+iInd);
 switch(u8_status_find)
  {
  case RG_START_FIND:
   {
   if(ch==c_PRE1[i_POS_PRE1])  // "weather-value";    
    i_POS_PRE1++;
    else
    {
    if(i_POS_PRE1<strlen(c_PRE1))
     i_POS_PRE1=0;
     else
     u8_status_find=RG_FIND_PRE2;
    }
   };break;

  case RG_FIND_PRE2:
   {
   if(ch==c_PRE2[i_POS_PRE2]) // "unit_temperature_c";
    i_POS_PRE2++;
    else
    {
    if(i_POS_PRE2<strlen(c_PRE2))
     i_POS_PRE2=0;
     else
     u8_status_find=RG_FIND_PRE3;
    }
   };break;

  case RG_FIND_PRE3:
   {
   if(ch==c_PRE3[i_POS_PRE3]) // "sign\">";   ="sign">+</span> , грузим знак, если есть и дальше...
    i_POS_PRE3++;
    else
    {
    if(i_POS_PRE3<strlen(c_PRE3))
     i_POS_PRE3=0;
     else
     {
     if(ch=='-')
      i_sign=-1;
      else
      i_sign=1;
     u8_status_find=RG_FIND_PRE4;
     }
    }
   };break;

  case RG_FIND_PRE4:
   {
   if(ch==c_PRE4[i_POS_PRE4]) // "</span>"; далее идёт число - температура  </span>20<span
    i_POS_PRE4++;
    else
    {
    if(i_POS_PRE4<strlen(c_PRE4))
     i_POS_PRE4=0;
     else
     {
     buf_temperatura[u8_buf_pos_temperatura++]=ch;
     u8_status_find=RG_FIND_PRE5;
     }
    }
   };break;

  case RG_FIND_PRE5:
   {
   if(ch==c_PRE5[i_POS_PRE5]) // "<";
    i_POS_PRE5++;
    else
    {
    if(i_POS_PRE5<strlen(c_PRE5) && u8_buf_pos_temperatura<10)
     {
     i_POS_PRE5=0;
     buf_temperatura[u8_buf_pos_temperatura++]=ch;
     }
     else
     u8_status_find=RG_FIND_PRE6;
    }
   };break;

  case RG_FIND_PRE6:
   {
   if(ch==c_PRE6[i_POS_PRE6]) // "temperature_f"; end
    i_POS_PRE6++;
    else
    {
    if(i_POS_PRE6<strlen(c_PRE6) && u8_buf_pos_temperatura<10)
     i_POS_PRE6=0;
     else
     {
     buf_temperatura[u8_buf_pos_temperatura++]=0x00;
     i_temperature_c=atoi(buf_temperatura);
     u8_status_find=RG_FIND_END;
     tls_get_rtc(&t_last_query);
     }
    }

   if(ch==c_PRE7[i_POS_PRE7]) // "lower\">,"; 
    i_POS_PRE7++;
    else
    {
    if(i_POS_PRE7<strlen(c_PRE7))
     i_POS_PRE7=0;
     else
     {
     if(i_POS_PRE7==strlen(c_PRE7))i_temperature_mantissa_c=ch-0x30;
     i_POS_PRE7++;
     }
    }

   };break;

  }
 }
}