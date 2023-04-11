#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mod1/UTFT.h"

#define LUAT_LOG_TAG "log"
#include "luat_log.h"

#ifndef LUAT_WEAK
#define LUAT_WEAK __attribute__((weak))
#endif

#define LOGLOG_SIZE 100
static char log_printf_buff[LOGLOG_SIZE];

LUAT_WEAK void luat_log_printf(int level, const char* _fmt, ...) {
    size_t len;
    va_list args;
    //if (luat_log_level_cur > level) return;
    va_start(args, _fmt);
    len = vsnprintf(log_printf_buff, LOGLOG_SIZE, _fmt, args);
    va_end(args);
    if (len > 0) {
        //luat_log_write(log_printf_buff, len);
        printf ("%s\n", log_printf_buff);
    }
}

static FIL fnew;       // file object

FILE* luat_fs_fopen(const char *filename, const char *mode)
{
  FRESULT res_sd; // file operation results
  // Open the file
  res_sd = f_open (&fnew, filename, FA_OPEN_EXISTING | FA_READ);
  // file opened successfully?
  if (res_sd == FR_OK)
    return &fnew;    
  else
    return NULL;
}


size_t luat_fs_fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{ 
  FRESULT res_sd; // file operation results
  UINT fnum=0;      // The number of files successfully read and written
  res_sd = f_read (stream, ptr, size*nmemb , &fnum);
  if (res_sd == FR_OK)
   return fnum;
  else 
   return 0;
}

int luat_fs_fseek(FILE* stream, long int offset, int origin)
{ 
  FRESULT res_sd; // file operation results
                  res_sd = f_lseek (
                      stream,         /* [IN] File object */
                      offset /* [IN] Offset of file read/write pointer
                                        to be set */
                  );
  if (res_sd == FR_OK)
   return 0;
  else 
   return 1;
   
}

int luat_fs_fclose(FILE* stream)
{ 
  f_close (stream);
  return 0;
};


int luat_lcd_draw(luat_lcd_conf_t* conf, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, luat_color_t* color)
{ 
UTFT_drawBitmap (x1, y1, x2, y2, color,  0);
return 0;
};

 
