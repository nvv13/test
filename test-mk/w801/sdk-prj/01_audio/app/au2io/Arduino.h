

#ifndef ARDUINO_H
#define ARDUINO_H

#if defined(__cplusplus)
extern "C"
{
#endif

#include "wm_type_def.h"

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_io.h"

#include "stddef.h"
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include <math.h>


#define pgm_read_word(data) *data
#define pgm_read_byte(data) *data
#define bitmapdatatype unsigned short *

#define PROGMEM
#define fontdatatype const unsigned char

//#define regtype volatile uint32_t
#define regtype uint32_t
#define regsize u8

#define regsize u8

#define boolean bool

#define uint u32

#define vTaskDelay tls_os_time_delay

uint32_t millis (void);

// void log_e(char * log){};
#define log_e printf
#define log_i printf

//char toupper (char a);
char toLowerCase (char a);
char *strlwr (char *str);

#define PI 3.1415926535897932384626433832795
void log_w (const char *log); // for debug



//**********************************************************//
bool psramFound (void);
bool psramInit (void);
u8 *ps_calloc (size_t buf_size, size_t len);
u8 *ps_malloc (size_t len);


#define MALLOC_CAP_DEFAULT  0
#define MALLOC_CAP_INTERNAL 1
#define MALLOC_CAP_SPIRAM   2

void * __malloc_heap_psram(size_t len);
void * heap_caps_malloc_prefer(u32 size, u8 a, u8 opt, u8 opt2);

//**********************************************************//


#if defined(__cplusplus)
}
#endif

#endif /* ARDUINO_H */
