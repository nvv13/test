#ifndef MY_RECOGNIZE_H
#define MY_RECOGNIZE_H

void my_recognize_http_reset(void);
void my_recognize_http_error(void);
void my_recognize_http(const char * recvbuf, int i_len);

#define MY_RECOGNIZE_NO_VALUE -100000

#include "wm_rtc.h"

int my_recognize_ret_cur_temperature_sign(void);
int my_recognize_ret_cur_temperature(void);
int my_recognize_ret_cur_temperature_mantissa(void);
struct tm my_recognize_ret_t_last_query(void);

#endif /* MY_RECOGNIZE_H */