/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: pin PB1, freq = 5,714309 MHz CPU_CLK_240M
 *
 * Date : 2022-05-20
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"

#include "wm_cpu.h"
#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_io.h"
#include "wm_osal.h"
#include "wm_psram.h"
#include "wm_regs.h"
#include "wm_rtc.h"
#include "wm_pmu.h"

#include "psram.h"

void
UserMain (void)
{
  printf ("user task test psram\n");

  // tls_sys_clk_set(CPU_CLK_2M);
  // tls_sys_clk_set (CPU_CLK_40M);
  // tls_sys_clk_set(CPU_CLK_80M);
  // tls_sys_clk_set(CPU_CLK_160M);
  tls_sys_clk_set (CPU_CLK_240M); // freq = 5,714309 MHz

  tls_sys_clk sysclk;
  tls_sys_clk_get (&sysclk);

  printf ("  sysclk.apbclk %d\n", sysclk.apbclk);
  printf ("  sysclk.apbclk*UNIT_MHZ %d\n", sysclk.apbclk * UNIT_MHZ);

  printf ("  sysclk.cpuclk %d\n", sysclk.cpuclk);
  printf ("  sysclk.cpuclk*UNIT_MHZ %d\n", sysclk.cpuclk * UNIT_MHZ);

  /**
   * @brief  config the pins used for psram ck cs dat0 dat1 dat2 dat3
   * @param  numsel: config psram ck cs dat0 dat1 dat2 dat3 pins multiplex
         relation,valid para 0,1

* 0:                 1: only for 56pin
* psram_ck   PB00    psram_ck   PA15
* psram_cs   PB01    psram_cs   PB27
* psram_dat0 PB02    psram_dat0 PB02
* psram_dat1 PB03    psram_dat1 PB03
* psram_dat2 PB04    psram_dat2 PB04
* psram_dat3 PB05    psram_dat3 PB05

   * @return None
  void wm_psram_config(uint8_t numsel);
   */

  wm_psram_config (1);

  //чтоб там инициализация DMA была, мало ли пригодится
  psram_init (
     PSRAM_SPI ); // PSRAM_QPI - почему то не заработал, что то не так сделал

  //инициализация с учетом частоты микросхемы LY68L6400, 80MHz, большенство таких.

  //d_psram_init (
  //   PSRAM_QPI,0,0 );

  d_psram_check();

  tls_os_time_delay (HZ);

  /*
  If PSRAM works in SPI mode, when writing 1 to PSRAM_CTRL[1], the controller
  will send command 35H to PSRAM, When PSRAM_CTRL[1] is read as 1, it means
  that the command is sent and the PSRAM enters the QPI mode. 16 SCLK cycles,
  22 SCLK cycles are required to complete a read operation. If PSRAM works in
  QPI mode, when writing 0 to PSRAM_CTRL[1], the controller will send command
  F5H to PSRAM, When PSRAM_CTRL[1] is read as 0, it indicates that the command
  transmission is completed, and the PSRAM enters the SPI mode. The PSRAM
  working mode must be set after the initialization operation is completed, but
  cannot be set at the same time
  */

  /**
   * @brief          This function is used to Copy block of memory in dma mode
  .
   *
   * @param[in]      src   Pointer to the source of data to be copied
   * @param[in]      dst   Pointer to the destination array where the content
  is to be copied
   * @param[in]      num   Number of bytes to copy
   *
   * @retval         num   Number of bytes that's been copied
   *
   * @note           None
  int memcpy_dma(unsigned char *dst, unsigned char *src, int num);
   */

  /*
  #define PSRAM_ADDR_START          0x30000000
  #define PSRAM_SIZE_BYTE           0x00800000 //  т.е. макс 8 388 608 байт
                                        у нас LY68L6400 памяти 64M Bits, или 8
  MB
  */
  //  char *test = dram_heap_malloc(1048576);
  //  char *test = dram_heap_malloc(2097152);
  // u32 i_size = 4194304;
  u32 i_size = 8 * 1048576;

  unsigned int t = 0; // used to save time relative to 1970
  struct tm *tblock;
  tblock = localtime ((const time_t *)&t); // switch to local time
  tls_set_rtc (tblock);
  struct tm tstart;
  struct tm tstop;

  u8 *test = (u8 *)PSRAM_ADDR_START;
  printf ("start test psram addr %d to size = %d \r\n", (int)test, i_size);

  u8 u8_cnt=10;
  while (u8_cnt--)
    {

      tls_get_rtc (&tstart);

      uint32_t cur = tls_os_get_time ();

      u8 b1 = 0;
      for (int i = 0; i < i_size; i++)
        {
          test[i] = b1++;
        }
      u8 b2_error = 0;
      b1 = 0;
      for (int i = 0; i < i_size; i++)
        {
          if (test[i] != b1++)
            {
              printf ("error pos = %d \r\n", i);
              b2_error = 1;
              break;
            };
        }
      if (b2_error)
        printf ("error test size = %d ", i_size);
      else
        printf ("ok test size = %d ", i_size);

      tls_get_rtc (&tstop);
      printf (
          "run test %d sec, %d tic \r\n",
          (tstop.tm_hour * 3600 + tstop.tm_min * 60 + tstop.tm_sec)
              - (tstart.tm_hour * 3600 + tstart.tm_min * 60 + tstart.tm_sec),
          tls_os_get_time () - cur);

      // strcpy(test,"hello1 world!!!\r\n");
      // printf("%s",test);
    }


  u8 buf[256];
  u8 buf2[256];
  strcpy((char*)buf,"hello1 world!!!\r\n");
  printf (" memcpy_dma test, buf  byte = %d  \r\n",
              memcpy_dma (test, buf, 256));
  printf("test %s",test);
  printf (" memcpy_dma buf2, test byte = %d  \r\n",
              memcpy_dma (buf2, test, 256));
  printf("buf2 %s",buf2);


  for (u8 i = 0; i <= 255; i++)
    {
      buf[i] = i;
      if (i == 255)
        break;
    }
  i_size = 256;
  printf ("fill buf \r\n");



  u8_cnt=100;
  while (u8_cnt--)
    {

      for (u8 i = 0; i <= 255; i++)
        {
          buf2[i] = 0;
          if (i == 255)
            break;
        }

      u8 b1 = u8_cnt;
      for (u8 i = 0; i <= 255; i++)
        {
          buf[i] = b1++;
          if (i == 255)
            break;
        }

      tls_get_rtc (&tstart);

      uint32_t cur = tls_os_get_time ();

      printf (" memcpy_dma test, buf  byte = %d  \r\n",
              memcpy_dma (test, buf, 256));
      printf (" memcpy_dma buf2, test byte = %d  \r\n",
              memcpy_dma (buf2, test, 256));

      u8 b2_error = 0;
      b1 = u8_cnt;
      for (int i = 0; i < i_size; i++)
        {
          if (buf2[i] != b1++)
            {
              printf ("error pos = %d \r\n", i);
              b2_error = 1;
              break;
            };
        }
      if (b2_error)
        printf ("error test size = %d ", i_size);
      else
        printf ("ok test size = %d ", i_size);

      tls_get_rtc (&tstop);
      printf (
          "run test %d sec, %d tic \r\n",
          (tstop.tm_hour * 3600 + tstop.tm_min * 60 + tstop.tm_sec)
              - (tstart.tm_hour * 3600 + tstart.tm_min * 60 + tstart.tm_sec),
          tls_os_get_time () - cur);

      // strcpy(test,"hello1 world!!!\r\n");
      // printf("%s",test);
    }

  i_size = 8 * 1048576 - 1024;

  test = dram_heap_malloc (i_size);
  printf ("start test psram addr %d to size = %d \r\n", (int)test, i_size);
  while (1)
    {

      tls_get_rtc (&tstart);

      uint32_t cur = tls_os_get_time ();

      u8 b1 = 0;
      for (int i = 0; i < i_size; i++)
        {
          test[i] = b1++;
        }
      u8 b2_error = 0;
      b1 = 0;
      for (int i = 0; i < i_size; i++)
        {
          if (test[i] != b1++)
            {
              printf ("error pos = %d \r\n", i);
              b2_error = 1;
              break;
            };
        }
      if (b2_error)
        printf ("error test size = %d ", i_size);
      else
        printf ("ok test size = %d ", i_size);

      tls_get_rtc (&tstop);
      printf (
          "run test %d sec, %d tic \r\n",
          (tstop.tm_hour * 3600 + tstop.tm_min * 60 + tstop.tm_sec)
              - (tstart.tm_hour * 3600 + tstart.tm_min * 60 + tstart.tm_sec),
          tls_os_get_time () - cur);

      // strcpy(test,"hello1 world!!!\r\n");
      // printf("%s",test);
    }
  dram_heap_free (test);
}
