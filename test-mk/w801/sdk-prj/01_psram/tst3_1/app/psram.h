/*

Взято из
https://gitee.com/beewu123/w80x-sdk-lcd.git
там Apache License  Version 2.0, January 2004

также, из взята инициализация
https://hackaday.io/project/189549-azzduino/details

*/

#ifndef __PSRAM_H__
#define __PSRAM_H__

void *dram_heap_malloc (size_t alloc_size);

void dram_heap_free (void *pfree);

size_t dram_heap_free_size (void);

size_t dram_heap_free_size_min (void);

int dram_heap_check_addr (void *addr);

#include "wm_psram.h"


void d_psram_init(psram_mode_t mode, u8 frequency_divider,u8 tCPH, u8 BURST, u16 OVERTIMER);
bool d_psram_check(void);

#endif