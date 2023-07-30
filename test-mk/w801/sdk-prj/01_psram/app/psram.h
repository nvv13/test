/*

Взято из
https://gitee.com/beewu123/w80x-sdk-lcd.git
там Apache License  Version 2.0, January 2004
*/

#ifndef __PSRAM_H__
#define __PSRAM_H__

void *dram_heap_malloc (size_t alloc_size);

void dram_heap_free (void *pfree);

size_t dram_heap_free_size (void);

size_t dram_heap_free_size_min (void);

int dram_heap_check_addr (void *addr);

#endif