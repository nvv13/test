#ifndef MY_RECOGNIZE_H
#define MY_RECOGNIZE_H

//#define HTTPS_DEMO_SERVER "www.gismeteo.ru"                 залочили сайт
// CloudFlare #define HTTPS_DEMO_GET "/weather-arkhangelsk-3915/now/"

//#define HTTPS_DEMO_SERVER "world-weather.ru"                защита капчей
//#define HTTPS_DEMO_GET "/pogoda/russia/arkhangelsk/"

#define HTTPS_DEMO_SERVER "m.rp5.ru"
#define HTTPS_DEMO_GET                                                        \
  "/%D0%9F%D0%BE%D0%B3%D0%BE%D0%B4%D0%B0_%D0%B2_%D0%90%D1%80%D1%85%D0%B0%D0%" \
  "BD%D0%B3%D0%B5%D0%BB%D1%8C%D1%81%D0%BA%D0%B5,_%D0%90%D1%80%D1%85%D0%B0%"   \
  "D0%BD%D0%B3%D0%B5%D0%BB%D1%8C%D1%81%D0%BA%D0%B0%D1%8F_%D0%BE%D0%B1%D0%BB%" \
  "D0%B0%D1%81%D1%82%D1%8C"
#define HTTPS_DEMO_PORT 443

void my_recognize_http_reset (void);
void my_recognize_http_error (void);
void my_recognize_http (const char *recvbuf, int i_len);

#define MY_RECOGNIZE_NO_VALUE -100000

#include "wm_rtc.h"

int my_recognize_ret_cur_temperature_sign (void);
int my_recognize_ret_cur_temperature (void);
int my_recognize_ret_cur_temperature_mantissa (void);
struct tm my_recognize_ret_t_last_query (void);
char * my_recognize_ret_buf_temperature (void);

#endif /* MY_RECOGNIZE_H */