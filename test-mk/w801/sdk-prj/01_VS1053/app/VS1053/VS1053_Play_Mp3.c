#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"
#include "wm_watchdog.h"

#include "ConsoleLogger.h"

#include "VS1053.h"

volatile enum VS1053_status my_sost = VS1053_NONE;

enum VS1053_status
VS1053_status_get_status (void)
{
  return my_sost;
};
void
VS1053_stop_PlayMP3 (void)
{
  if (my_sost != VS1053_STOP)
    my_sost = VS1053_QUERY_TO_STOP;
};

// pack(push,1) - Byte alignment ?
#pragma pack(push, 1)
// MP3 Header
typedef struct tagMP3
{
  char ID3[3]; // = "ID3"
  u8 ver;
  u8 sum_ver;
  u8 flag;
  u8 header_size[4];

} MP3HDR, *PMP3HDR;
#pragma pack(pop)

static FIL fnew;       // file object
static FRESULT res_sd; // file operation results
static UINT fnum;      // The number of files successfully read and written

#define DEMO_DATA_SIZE 32 // 4096
static u8 file_buffer[DEMO_DATA_SIZE] = { 0 };
#define SERIAL_DEBUG
//#define SERIAL_DEBUG_ALL

FRESULT
VS1053_PlayMp3 (char *filename)
{

  VS1053_switchToMp3Mode (); // optional, some boards require this (softReset
                             // include!)

  uint32_t start;

  // Open the file
  res_sd = f_open (&fnew, filename, FA_OPEN_EXISTING | FA_READ);
  // file opened successfully?
  if (res_sd == FR_OK)
    {
#ifdef SERIAL_DEBUG_ALL
      printf ("Open file successfully! Start reading data!\r\n");
#endif
      res_sd = f_read (&fnew, file_buffer, sizeof (MP3HDR), &fnum);
      if (res_sd == FR_OK)
        {
          // Parse BMP header to get the information we need
          PMP3HDR aHead = (PMP3HDR)file_buffer;
          if (strstr (aHead->ID3, "ID3") != NULL)
            {
#ifdef SERIAL_DEBUG_ALL
              printf ("MP3 header ok\r\n");
#endif
              if (!(aHead->ver > 0 && aHead->ver < 6))
                {
                  printf ("Wrong `version` value %d\r\n", aHead->ver);
                  f_close (&fnew);
                  return -7;
                }

              start = 0ul;
              start |= (0x7F & aHead->header_size[0]);
              start <<= 7;
              start |= (0x7F & aHead->header_size[1]);
              start <<= 7;
              start |= (0x7F & aHead->header_size[2]);
              start <<= 7;
              start |= (0x7F & aHead->header_size[3]);

              res_sd = f_lseek (&fnew, start);
#ifdef SERIAL_DEBUG
              printf ("f_lseek successfully! %d\r\n", start);
#endif

              fnum = vs1053_chunk_size;
              my_sost = VS1053_PLAY;
              //(2)VS1053_data_mode_on ();
              while (fnum == vs1053_chunk_size
                     && my_sost == VS1053_PLAY) // More to do?
                {
                  //(2)VS1053_await_data_request (); // Wait for space
                  // available
                  res_sd
                      = f_read (&fnew, file_buffer, vs1053_chunk_size, &fnum);
                  //(2)SPI_writeBytes (file_buffer, fnum);
                  VS1053_playChunk ((u8 *)file_buffer, fnum);
                }
              //(2)VS1053_data_mode_off ();
              my_sost = VS1053_STOP;
            }
        }
      // close file
      f_close (&fnew);
    }
  return res_sd;
}
