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
#include <message_buffer.h>

#include "HTTPClient.h"
#include "wm_type_def.h"
#include "wm_watchdog.h"

#include "ConsoleLogger.h"

#include "psram.h"
#include "VS1053.h"

u32 VS1053_WEB_RADIO_nTotal = 0;

#define HTTP_1053_SERIAL_DEBUG

#define http_chunk_size (vs1053_chunk_size * 2)
#define HTTP_CLIENT_BUFFER_SIZE (http_chunk_size * 2)

#define VS1053_TASK_SIZE 1024
tls_os_task_t vs1053_buf_play_task_hdl = NULL;
static OS_STK vs1053_buf_playTaskStk[VS1053_TASK_SIZE];
#define VS1053_TASK_PRIO 32


static volatile u32 xMessageBufferSize=4000;
  // 4000 - не вызывает проблем, но эффект начинаеться от 8000,
  //   но тут глючит прога
  // а чтоб нормально работал HTTPS  нужно 250Кб, примено
static uint8_t * ucStorageBuffer = NULL; // [ xMessageBufferSize+1 ];
/* The variable used to hold the message buffer structure. */
StaticMessageBuffer_t xMessageBufferStruct;
MessageBufferHandle_t xMessageBuffer = NULL;


void
vs1053_buf_play_task (void *sdata)
{
  printf ("start vs1053_buf_play_task\n");

  u8 *buffer = (u8 *)tls_mem_alloc (HTTP_CLIENT_BUFFER_SIZE);

  while (1)
    {
      size_t item_size;
      while (xMessageBuffer != NULL && buffer != NULL /* && my_sost != VS1053_STOP && my_sost != VS1053_QUERY_TO_STOP */
              )
        {
          if (my_sost == VS1053_PLAY || my_sost == VS1053_PLAY_BUF)
            {
              item_size = xMessageBufferReceive (
                  xMessageBuffer, buffer, HTTP_CLIENT_BUFFER_SIZE, portMAX_DELAY);
              if (item_size > 0)
                {
                VS1053_playChunk (buffer, item_size);
#ifdef HTTP_1053_SERIAL_DEBUG
                printf ("-");
#endif
                if (VS1053_WEB_RADIO_nTotal > 512)
                   tls_watchdog_clr ();
                }
              else
                {
                tls_os_time_delay (10);
#ifdef HTTP_1053_SERIAL_DEBUG
                printf ("0");
#endif
                }
            }
          else
            tls_os_time_delay (HZ / 100);
        }
      tls_os_time_delay (HZ / 100);
    }

  if (buffer != NULL)
    tls_mem_free (buffer);

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
  if(my_sost != VS1053_PLAY_BUF)my_sost = VS1053_HW_INIT;
  VS1053_WEB_RADIO_nTotal = 0;

  Buffer = (char *)tls_mem_alloc (HTTP_CLIENT_BUFFER_SIZE);
  if (Buffer == NULL)
    {
      return HTTP_CLIENT_ERROR_NO_MEMORY;
    }
  memset (Buffer, 0, HTTP_CLIENT_BUFFER_SIZE);
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

  P_HTTP_SESSION  pHTTPSession = (P_HTTP_SESSION)pHTTP;
  pHTTPSession->HttpFlags = pHTTPSession->HttpFlags | HTTP_CLIENT_FLAG_KEEP_ALIVE;

  if (xMessageBuffer == NULL)
    {
    wm_psram_config (1);
    d_psram_init (PSRAM_SPI,2,2,1,2);
    tls_os_time_delay (HZ/10);
    if(d_psram_check())
      {
      xMessageBufferSize=(HTTP_CLIENT_BUFFER_SIZE+4)*200;//256000;//больще не надо! глючит! 
      ucStorageBuffer = dram_heap_malloc (xMessageBufferSize+1);
      }
      else
      {
      xMessageBufferSize=4000;
      ucStorageBuffer = (u8 *)tls_mem_alloc (xMessageBufferSize+1);
      }
    if (ucStorageBuffer == NULL)
      {
        return HTTP_CLIENT_ERROR_NO_MEMORY;
      }
    xMessageBuffer = xMessageBufferCreateStatic( xMessageBufferSize,
                         ucStorageBuffer,  &xMessageBufferStruct );
    printf ("xMessageBufferSize=%d\r\n", xMessageBufferSize);
    }



  if (vs1053_buf_play_task_hdl == NULL)
    tls_os_task_create (
        &vs1053_buf_play_task_hdl, NULL, vs1053_buf_play_task, NULL,
        (void *)vs1053_buf_playTaskStk,  /* task's stack start address */
        VS1053_TASK_SIZE * sizeof (u32), /* task's stack size, unit:byte */
        VS1053_TASK_PRIO, 0);

  //if(my_sost != VS1053_PLAY_BUF)
  my_sost = VS1053_HW_INIT;
  do
    {
      if(my_sost != VS1053_HW_INIT)my_sost = VS1053_PLAY_BUF;
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
      tls_os_time_delay (HZ/3);

      u16 u16_connect_timeout_sec = 300;
      if (strstr (ClientParams.Uri, "https") != NULL)
        u16_connect_timeout_sec = 600;

      printf ("Start to receive data from remote server\r\n");

      //int i_cnt=0;
      //сначала заполняем буффер данными
      while (
          (nRetCode == HTTP_CLIENT_SUCCESS /* || nRetCode != HTTP_CLIENT_EOS*/)
          //&& my_sost == VS1053_HW_INIT
          )
        {
          nSize = HTTP_CLIENT_BUFFER_SIZE;
          size_t freeSize = xMessageBufferSpacesAvailable (xMessageBuffer);
          //ptintf ("%d ", freeSize);
          if (freeSize < (nSize + 4))// || i_cnt++>4096)
            {
            printf ("PreLoad buf, freeSize=%d\r\n", freeSize);
            break;
            }
          u32 u_fur=tls_os_get_time();
          nRetCode = HTTPClientReadData (pHTTP, Buffer, nSize,
                                         u16_connect_timeout_sec, &nSize);
          if(tls_os_get_time()-u_fur-2<HZ)tls_os_time_delay (tls_os_get_time()-u_fur-2);
          if (nRetCode == HTTP_CLIENT_SUCCESS)
            {
              xMessageBufferSend (xMessageBuffer, Buffer, nSize,
                                  portMAX_DELAY);
#ifdef HTTP_1053_SERIAL_DEBUG
              printf ("f");
#endif
              tls_watchdog_clr ();
            }
        }
      tls_os_time_delay (100);

      if (my_sost != VS1053_QUERY_TO_STOP)
        my_sost = VS1053_PLAY;

      VS1053_WEB_RADIO_nTotal = 0;
      // Get the data until we get an error or end of stream code
      while (
          (nRetCode == HTTP_CLIENT_SUCCESS /* || nRetCode != HTTP_CLIENT_EOS*/)
          && my_sost == VS1053_PLAY)
        {
          // Set the size of our buffer
          nSize = HTTP_CLIENT_BUFFER_SIZE;
          // Get the data
          u32 u_fur=tls_os_get_time();
          nRetCode = HTTPClientReadData (pHTTP, Buffer, nSize,
                                         u16_connect_timeout_sec, &nSize);
          if (nRetCode != HTTP_CLIENT_SUCCESS && nRetCode != HTTP_CLIENT_EOS)
            break;
          if(tls_os_get_time()-u_fur<HZ)tls_os_time_delay (tls_os_get_time()-u_fur);

          //ждем свободное место в буфере, и заполняем его
          while (my_sost == VS1053_PLAY)
            {
              size_t freeSize = xMessageBufferSpacesAvailable (xMessageBuffer);
              if (freeSize > (nSize + 4))
                {
                  xMessageBufferSend (xMessageBuffer, Buffer, nSize,
                                      portMAX_DELAY);
                  //printf (" %d ", freeSize);
#ifdef HTTP_1053_SERIAL_DEBUG
                  printf ("+");
#endif
                  break;
                }
              tls_os_time_delay (HZ / 100);
            }

          //if (VS1053_WEB_RADIO_nTotal > 512)
          //  tls_watchdog_clr ();
          VS1053_WEB_RADIO_nTotal += nSize;
        }
    }
  while (my_sost == VS1053_PLAY); // Run only once

  if(my_sost != VS1053_PLAY_BUF)
   {
   my_sost = VS1053_STOP;
   tls_os_time_delay (HZ);
   xMessageBufferReset (xMessageBuffer);
   }
  tls_mem_free (Buffer);

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