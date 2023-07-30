#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "wm_type_def.h"
//#include "wm_uart.h"
//#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
//#include "wm_hostspi.h"
//#include "wm_socket.h"
//#include "wm_sockets.h"
//#include "wm_wifi.h"
//#include "wm_hspi.h"
//#include "wm_pwm.h"
//#include "wm_params.h"
//#include "wm_osal.h"
//#include "wm_netif.h"
//#include "wm_efuse.h"
//#include "wm_mem.h"
//#include "wm_cpu.h"
//#include "wm_regs.h"
//#include "wm_rtc.h"
//#include "wm_timer.h"
//#include "wm_watchdog.h"
//#include "csi_core.h"

#include <FreeRTOS.h>
#include <task.h>
//#include <message_buffer.h>

#include "HTTPClient.h"
#include "wm_type_def.h"
#include "wm_watchdog.h"

#include "ConsoleLogger.h"

#include "VS1053.h"

#include "wm_psram.h"
//#include "psram.h"

//**********************************************************************************************************************************//

#define BUF_SIZE                                                              \
  128 //размер буфера обязательно должен быть равен степени двойки!
#define BUF_MASK (BUF_SIZE - 1)
#define BUF_BYTE_CHUNK_SIZE 64 //

// static bool b_psram = true;
static volatile int idxIN = 0;
static volatile int idxOUT = 0;
static volatile int idxUSE = 0;
static u8 *ring_buf = NULL; // [BUF_SIZE*BUF_BYTE_CHUNK_SIZE]; //  = NULL; //

static u8 *
CreateRingBuf (void)
{
  taskENTER_CRITICAL ();
  //  if (b_psram)
  //    {
  //      wm_psram_config (0);
  //      psram_init (PSRAM_SPI); // PSRAM_QPI - почему то не заработал, что то
  //      не
  //                              // так сделал
  //      ring_buf = (u8 *)PSRAM_ADDR_START;
  //    }
  //  else
  //    {
  ring_buf = tls_mem_alloc (BUF_SIZE * BUF_BYTE_CHUNK_SIZE);
  //    }
  printf (" CreateRingBuf address= %d, size %d \n\n", (int)ring_buf,
          BUF_SIZE * BUF_BYTE_CHUNK_SIZE);
  idxIN = 0;
  idxOUT = 0;
  idxUSE = 0;
  taskEXIT_CRITICAL ();
  return ring_buf;
}

static u8 *
PushRingBuf (void)
{
  u8 *bb_put = NULL;
  taskENTER_CRITICAL ();
  if (ring_buf != NULL && idxUSE < BUF_SIZE)
    {
      //При помещении значения value в буфер используется индекс idxIN. Это
      //делается так:
      //  if (b_psram)
      //    memcpy_dma (ring_buf + (idxIN * BUF_BYTE_CHUNK_SIZE), Buffer,
      //                BUF_BYTE_CHUNK_SIZE);
      //  else
      //    memcpy (ring_buf + (idxIN * BUF_BYTE_CHUNK_SIZE), Buffer,
      //            BUF_BYTE_CHUNK_SIZE);
      bb_put = ring_buf + (idxIN * BUF_BYTE_CHUNK_SIZE);
      //printf (" PushRingBuf address= %d, idxIN = %d, idxUSE=%d\n\n",(int)bb_put,idxIN,idxUSE);
      idxIN++;
      idxIN &= BUF_MASK;
      idxUSE++;
    }
  taskEXIT_CRITICAL ();
  return bb_put;
}

static u8 *
PopRingBuf (void)
{
  u8 *bb_get = NULL;
  taskENTER_CRITICAL ();
  if (ring_buf != NULL && idxUSE > 1)
    {
      //Операция выборки из буфера происходит похожим образом, только
      //используется индекс idxOUT:
      //  if (b_psram)
      //    memcpy_dma (Buffer, ring_buf + (idxOUT * BUF_BYTE_CHUNK_SIZE),
      //                BUF_BYTE_CHUNK_SIZE);
      //  else
      //    memcpy (Buffer, ring_buf + (idxOUT * BUF_BYTE_CHUNK_SIZE),
      //            BUF_BYTE_CHUNK_SIZE);
      bb_get = ring_buf + (idxOUT * BUF_BYTE_CHUNK_SIZE);
      //printf (" PopRingBuf address= %d, idxOUT = %d, idxUSE=%d \n\n",(int)bb_get,idxOUT,idxUSE);
      idxOUT++;
      idxOUT &= BUF_MASK;
      idxUSE--;
    }
  taskEXIT_CRITICAL ();
  return bb_get;
}

static void
ResetRingBuf (void)
{
  taskENTER_CRITICAL ();
  //Сбросить данные буфера (т. е. удалить их оттуда) тоже очень просто - для
  //этого в idxOUT записывают значение idxIN: idxOUT = idxIN;
  idxIN = 0;
  idxOUT = 0;
  idxUSE = 0;
  printf (" ResetRingBuf address= %d, size %d \n\n", (int)ring_buf,
          BUF_SIZE * BUF_BYTE_CHUNK_SIZE);
  taskEXIT_CRITICAL ();
  // printf (" ResetRingBuf \n\n");
}

//сколько данных в буфере.
// static u8
// idxDiff ()
//{
//  return (idxIN - idxOUT) & BUF_MASK;
//
//    if (idxIN >= idxOUT)
//        return (idxIN - idxOUT);
//    else
//        return ((BUF_SIZE - idxOUT) + idxIN);
//
//}

static int
RingBufSpacesAvailable (void)
{
  int i_size = 0;
  // printf (" RingBufSpacesAvailable idxIN = %d idxOUT = %d = %d\n\n",idxIN ,
  // idxOUT, ((idxIN - idxOUT) & BUF_MASK));
  //  return BUF_SIZE - ((idxIN - idxOUT) & BUF_MASK) - 1;
  // if((BUF_SIZE - idxUSE - 4)<=0) return 0;
  taskENTER_CRITICAL ();
  i_size = (BUF_SIZE - idxUSE); // * BUF_BYTE_CHUNK_SIZE;
  taskEXIT_CRITICAL ();
  return i_size;
}

//**********************************************************************************************************************************//

u32 VS1053_WEB_RADIO_nTotal = 0;

#define VS1053_TASK_SIZE 2048
static tls_os_task_t vs1053_buf_play_task_hdl = NULL;
static OS_STK vs1053_buf_playTaskStk[VS1053_TASK_SIZE];
#define VS1053_TASK_PRIO 32

static void
vs1053_buf_play_task (void *sdata)
{
  printf ("start vs1053_buf_play_task\n");

  u8 *buffer = NULL; //(u8 *)tls_mem_alloc (BUF_BYTE_CHUNK_SIZE);

  while (1)
    {
      // size_t item_size;
      while (ring_buf != NULL && my_sost != VS1053_STOP
             && my_sost != VS1053_QUERY_TO_STOP)
        {
          if (my_sost == VS1053_PLAY)
            {
              buffer = PopRingBuf ();
              if (buffer != NULL)
                VS1053_playChunk (buffer, BUF_BYTE_CHUNK_SIZE);
              else
                tls_os_time_delay (1);
            }
          else
            tls_os_time_delay (HZ / 100);
        }
      tls_os_time_delay (HZ / 100);
    }

  // if (buffer != NULL)
  //  tls_mem_free (buffer);

  printf ("stop vs1053_buf_play_task\n");
}

static u32
http_snd_req (HTTPParameters ClientParams, HTTP_VERB verb, char *pSndData,
              u8 parseXmlJson)
{
  int nRetCode;
  u32 nSize = 0;
  char *Buffer = NULL;
  HTTP_SESSION_HANDLE pHTTP;
  u32 nSndDataLen;
  my_sost = VS1053_HW_INIT;
  VS1053_WEB_RADIO_nTotal = 0;

  Buffer = (char *)tls_mem_alloc (BUF_BYTE_CHUNK_SIZE);
  if (Buffer == NULL)
    {
      return HTTP_CLIENT_ERROR_NO_MEMORY;
    }
  memset (Buffer, 0, BUF_BYTE_CHUNK_SIZE);
  printf ("HTTP Client v1.0\r\n");
  nSndDataLen = (pSndData == NULL ? 0 : strlen (pSndData));
  // Open the HTTP request handle
  pHTTP = HTTPClientOpenRequest (0);
  if (!pHTTP)
    {
      nRetCode = HTTP_CLIENT_ERROR_INVALID_HANDLE;
      tls_mem_free (Buffer);
      return nRetCode;
    }

  /*
if((nRetCode = HTTPClientAddRequestHeaders(pHTTP,"media type",
"application/json", 1))!= HTTP_CLIENT_SUCCESS)
  {
break;
}
  */
  // Set the Verb
  nRetCode = HTTPClientSetVerb (pHTTP, verb);
  if (nRetCode != HTTP_CLIENT_SUCCESS)
    {
      tls_mem_free (Buffer);
      return nRetCode;
    }
#if TLS_CONFIG_HTTP_CLIENT_AUTH
    // Set authentication
//        if(ClientParams.AuthType != AuthSchemaNone)
//        {
//            if((nRetCode = HTTPClientSetAuth(pHTTP, ClientParams.AuthType,
//            NULL)) != HTTP_CLIENT_SUCCESS)
//            {
//                break;
//            }
//
//            // Set authentication
//            if((nRetCode = HTTPClientSetCredentials(pHTTP,
//            ClientParams.UserName, ClientParams.Password)) !=
//            HTTP_CLIENT_SUCCESS)
//            {
//                break;
//            }
//        }
#endif // TLS_CONFIG_HTTP_CLIENT_AUTH
#if TLS_CONFIG_HTTP_CLIENT_PROXY
    // Use Proxy server
//        if(ClientParams.UseProxy == TRUE)
//        {
//            if((nRetCode = HTTPClientSetProxy(pHTTP, ClientParams.ProxyHost,
//            ClientParams.ProxyPort, NULL, NULL)) != HTTP_CLIENT_SUCCESS)
//            {
//                break;
//            }
//        }
#endif // TLS_CONFIG_HTTP_CLIENT_PROXY

  if (ring_buf == NULL)
    {
      CreateRingBuf ();
    }
  if (ring_buf == NULL)
    {
      tls_mem_free (Buffer);
      return HTTP_CLIENT_ERROR_NO_MEMORY;
    }

  if (vs1053_buf_play_task_hdl == NULL)
    {
      tls_os_task_create (
          &vs1053_buf_play_task_hdl, NULL, vs1053_buf_play_task, NULL,
          (void *)vs1053_buf_playTaskStk,  /* task's stack start address */
          VS1053_TASK_SIZE * sizeof (u32), /* task's stack size, unit:byte */
          VS1053_TASK_PRIO, 0);
    }

  do
    {
      my_sost = VS1053_HW_INIT;
      if ((nRetCode = HTTPClientSendRequest (
               pHTTP, ClientParams.Uri, pSndData, nSndDataLen,
               verb == VerbPost || verb == VerbPut, 0, 0))
          != HTTP_CLIENT_SUCCESS)
        {
          break;
        }
      // Retrieve the the headers and analyze them
      if ((nRetCode = HTTPClientRecvResponse (pHTTP, 30))
          != HTTP_CLIENT_SUCCESS)
        {
          break;
        }
      tls_os_time_delay (HZ);

      u16 u16_connect_timeout_sec = 10;
      if (strstr (ClientParams.Uri, "https") != NULL)
        u16_connect_timeout_sec = 30;

      printf ("Start to receive data from remote server\r\n");

      //сначала заполняем буффер данными
      while (nRetCode == HTTP_CLIENT_SUCCESS  && my_sost == VS1053_HW_INIT)
        {
          nSize = BUF_BYTE_CHUNK_SIZE;
          size_t freeSize = RingBufSpacesAvailable ();
          printf (" freeSize=%d\r\n", freeSize);
          if (freeSize == 0)
            break;
          nRetCode = HTTPClientReadData (pHTTP, PushRingBuf (), nSize,
                                         u16_connect_timeout_sec, &nSize);
          if (nRetCode == HTTP_CLIENT_SUCCESS)
            {
              // PushRingBuf ((u8 *)Buffer);
              tls_watchdog_clr ();
            }
        }

      if (my_sost != VS1053_QUERY_TO_STOP)
        my_sost = VS1053_PLAY;

      VS1053_WEB_RADIO_nTotal = 0;
      // Get the data until we get an error or end of stream code
      while (nRetCode == HTTP_CLIENT_SUCCESS && my_sost == VS1053_PLAY)
        {
          // Set the size of our buffer
          nSize = BUF_BYTE_CHUNK_SIZE;
          // Get the data

          //ждем свободное место в буфере, и заполняем его
          while (my_sost == VS1053_PLAY)
            {
              if (RingBufSpacesAvailable () > 1)
                break;
              tls_os_time_delay (HZ / 100);
            }
          nRetCode = HTTPClientReadData (pHTTP, PushRingBuf (), nSize,
                                         u16_connect_timeout_sec, &nSize);
          if (nRetCode != HTTP_CLIENT_SUCCESS && nRetCode != HTTP_CLIENT_EOS)
            break;

          if (VS1053_WEB_RADIO_nTotal > 512)
            tls_watchdog_clr ();
          VS1053_WEB_RADIO_nTotal += nSize;
        }
    }
  while (my_sost == VS1053_PLAY); // Run only once

  my_sost = VS1053_STOP;
  tls_os_time_delay (HZ);
  // if (vs1053_buf_play_task_hdl)
  //  {
  //    tls_os_task_del_by_task_handle (vs1053_buf_play_task_hdl,
  //                                    NULL /*task_vs1053_free*/);
  //    vs1053_buf_play_task_hdl = NULL;
  //  }
  tls_mem_free (Buffer);
  // vMessageBufferDelete (xMessageBuffer);
  ResetRingBuf ();

  if (pHTTP)
    HTTPClientCloseRequest (&pHTTP);
  if (ClientParams.Verbose == TRUE)
    {
      printf ("\n\nHTTP Client terminated %d (got %d b)\n\n", nRetCode,
              VS1053_WEB_RADIO_nTotal);
    }
  return nRetCode;
}

static u32
http_get (HTTPParameters ClientParams)
{
  return http_snd_req (ClientParams, VerbGet, NULL, 0);
}

FRESULT
VS1053_PlayHttpMp3 (const char *Uri)
{

  VS1053_switchToMp3Mode (); // optional, some boards require this (softReset
                             // include!)

  HTTPParameters httpParams;
  memset (&httpParams, 0, sizeof (HTTPParameters));
  httpParams.Uri = (char *)tls_mem_alloc (200);
  if (httpParams.Uri == NULL)
    {
      printf ("malloc error.\n");
      return WM_FAILED;
    }
  memset (httpParams.Uri, 0, 200);
  sprintf (httpParams.Uri, "%s", Uri);
  httpParams.Verbose = TRUE;
  printf ("Location: %s\n", httpParams.Uri);
  int res_sd = http_get (httpParams);
  tls_mem_free (httpParams.Uri);

  return res_sd;
}
