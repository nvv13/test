
#include "ff.h"
#include "mod1/lfile_http.h"
#include "mod1/psram.h"
#include "wm_gpio_afsel.h"

static FATFS fs;
static FIL fnew;
static FRESULT res_sd;
static UINT fnum;
static BYTE ReadBuffer[256] = { 0 };

static FRESULT
psram_disk_init (void)
{

  wm_psram_config (1);
  d_psram_init (PSRAM_SPI, 2, 2, 1, 2);
  tls_os_time_delay (HZ / 10);

  // mount psram disk ! не забудь заменить libapp на /mod1/libapp в makefile!
  res_sd = f_mount (&fs, "1:", 1);
  //***********************formatting test****************************
  if (res_sd == FR_NO_FILESYSTEM)
    {
      printf ("FR_NO_FILESYSTEM:Failed to mount file system! Probably "
              "because the file "
              "initialization failed! error code:%d\r\n",
              res_sd);

      BYTE work[FF_MAX_SS];
      res_sd = f_mkfs ("1:", 0, work, sizeof (work));

      if (res_sd == FR_OK)
        {
          printf ("mount again\r\n");
          res_sd = f_mount (NULL, "1:", 1);
          printf ("code:%d\r\n", res_sd);
          res_sd = f_mount (&fs, "1:", 1);
          printf ("code:%d\r\n", res_sd);
        }
      else
        {
          printf ("f_mkfs failed! error code:%d\r\n", res_sd);
        }
    }
  return res_sd;
}

static FRESULT
psram_disk_load_list (void)
{
  char FileName[20];
  sprintf (FileName, "1:test0.jpg");

  //char s_Url[1024];
  //sprintf (s_Url, "%s","https://disk.yandex.ru/d/PT1ai6H4rHEJzw"); // aUrl[ind_file]);
  //const char * s_Url="https://raw.githubusercontent.com/nvv13/test/main/android%20command-line%20tools.txt";
  //const char * s_Url="https://raw.githubusercontent.com/nvv13/test/main/README.md";
  //const char * s_Url="https://gitflic.ru/project/nvv13/test/blob/raw?file=test-mk%2Fw801%2Ftips.txt&inline=false&commit=645505eff0c493243485a430f7974ab7373ec67e";
  const char * s_Url="https://gitflic.ru/project/nvv13/test/blob/raw?file=test-mk%2Fw801%2Ftips.txt";
  printf ("s_Url = %s\n", s_Url);
  res_sd = f_open (&fnew, FileName, FA_CREATE_ALWAYS | FA_WRITE);
  if (res_sd == FR_OK)
    {
      printf ("f_open ok, try load url = %s \r\n", s_Url);
      // природа -inurl:https
      res_sd = download_file_http (s_Url, &fnew);

      //              res_sd
      //                  = f_write (&fnew, WriteBuffer, sizeof
      //                  (WriteBuffer), &fnum);
      if (res_sd == FR_OK)
        {
          // printf ("fnum = %d\r\n", fnum);
          // printf ("WriteBuffer = %s \r\n", WriteBuffer);
        }
      else
        {
          printf ("f_write failed! error code:%d\r\n", res_sd);
          tls_os_time_delay (HZ);
        }

      f_close (&fnew);

      res_sd = f_open (&fnew, FileName, FA_OPEN_EXISTING | FA_READ);
      if (res_sd == FR_OK)
        {
          printf ("f_open ok\r\n");
          res_sd = f_read (&fnew, ReadBuffer, sizeof (ReadBuffer), &fnum);

          if (res_sd == FR_OK)
            {
              printf ("fnum = %d\r\n", fnum);
              printf ("ReadBuffer = %s\r\n", ReadBuffer);
            }
          else
            {
              printf ("f_read failed! error code:%d\r\n", res_sd);
              tls_os_time_delay (HZ);
            }

          f_close (&fnew);

        }
      else
        {
          printf ("f_open failed! error code:%d\r\n", res_sd);
          tls_os_time_delay (HZ);
        }
    }
 return res_sd;
}


