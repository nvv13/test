

#if defined(__cplusplus)
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
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
  //#include "wm_regs.h"
#include "Arduino.h"

#if defined(__cplusplus)
}
#endif



#ifndef UINT32_MAX
#define UINT32_MAX (4294967295U)
#endif

/*Возвращает количество миллисекунд с момента начала выполнения текущей
  программы на плате Arduino.
  Это количество сбрасывается на ноль, в следствие переполнения значения,
  приблизительно через 50 дней.*/
uint32_t
millis (void)
{
  uint32_t cur = tls_os_get_time ();
  if (cur > (UINT32_MAX / (1000 / HZ)))
    cur -= (UINT32_MAX / (1000 / HZ));
  return cur * (1000 / HZ);
};

//extern "C" int HTTPWrapperToUpper (int c);
//char
//toupper (char a)
//{
//  return (char)HTTPWrapperToUpper (a);
//};

extern int HTTPWrapperToLower (int c);
char
toLowerCase (char a)
{
  return (char)HTTPWrapperToLower (a);
};

/* The strlwr( ) function is a built-in function in C and is used to convert a
 * given string into lowercase. */
char *
strlwr (char *str)
{
  char *str0 = str;
  do
    {
      *str0 = toLowerCase (*str0);
    }
  while ((*(str0++)) != 0);
  return str;
};

bool
psramFound (void)
{
  return false;
}; //будем считать что не подключена
bool
psramInit (void)
{
  return false;
};
u8 *
ps_calloc (size_t buf_size, size_t len)
{
  return NULL;
};
u8 *
ps_malloc (size_t len)
{
  return NULL;
};

void
log_w (const char *log)
{
  printf ("%s\n", log);
}; // for debug


void * __malloc_heap_psram(size_t len){
return NULL;
};

void * heap_caps_malloc_prefer(u32 size, u8 a, u8 opt, u8 opt2){return NULL;};



