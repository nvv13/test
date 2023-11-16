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

#include "mod1/psram.h"
#include "VS1053.h"


volatile int VS1053_WEB_RADIO_nTotal = 0;
volatile int VS1053_WEB_RADIO_buf_chunk_free = 0;
volatile int VS1053_WEB_RADIO_buf_chunk_total = 0;
#define DF_VS1053_WEB_RADIO_buf_chunk_top 300

//#define http_chunk_size (vs1053_chunk_size * 2)
#define HTTP_CLIENT_BUFFER_SIZE (vs1053_chunk_size * 4)


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



static    u16 no_psram_BufferSize=4000;// подойдет 4000, более - программа начнет глючить
static    u32 psram_BufferSize=(HTTP_CLIENT_BUFFER_SIZE+4)*200;   // подойдет 26400 более не надо! глючит! 
static    u8 psram_config=1;//0 или 1 
static    psram_mode_t psram_mode=PSRAM_SPI;// делай PSRAM_SPI, PSRAM_QPI - так и не работает
static    u8 psram_frequency_divider=2;//2 - хорошо работает для ESP-PSRAM64H 
static    u8 psram_tCPH=2;//2 - хорошо работает для ESP-PSRAM64H 
static    u8 psram_BURST=1;//1 - хорошо работает для ESP-PSRAM64H 
static    u16 psram_OVERTIMER=2;//2 - хорошо работает для ESP-PSRAM64H 
static    u8 load_buffer_debug=0;//0 или 1 

void VS1053_PlayHttpMp3_set (libVS1053_t *set_pin)
{
    load_buffer_debug=set_pin->load_buffer_debug;

    if(set_pin->no_psram_BufferSize>0)
        no_psram_BufferSize=set_pin->no_psram_BufferSize;

    if(set_pin->psram_BufferSize>0)
     {
        psram_BufferSize=set_pin->psram_BufferSize;   
        psram_config=set_pin->psram_config;
        psram_mode=set_pin->psram_mode;
        psram_frequency_divider=set_pin->psram_frequency_divider;
        psram_tCPH=set_pin->psram_tCPH;
        psram_BURST=set_pin->psram_BURST;
        psram_OVERTIMER=set_pin->psram_OVERTIMER;
     }
}



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
                if(load_buffer_debug)
                   printf ("-");
                VS1053_WEB_RADIO_buf_chunk_free--;
                if (VS1053_WEB_RADIO_nTotal > 512)
                   tls_watchdog_clr ();
                }
              else
                {
                tls_os_time_delay (1);
                if(load_buffer_debug)
                   printf ("0");
                }
            }
          else
            tls_os_time_delay (10);
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

  //Icy-MetaData:1
//  if((nRetCode = HTTPClientAddRequestHeaders(pHTTP,"Icy-MetaData","1", 1))!= HTTP_CLIENT_SUCCESS)
//    {
//      tls_mem_free (Buffer);
//      return nRetCode;
//    }


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
    wm_psram_config (psram_config);
    d_psram_init (psram_mode,psram_frequency_divider,psram_tCPH,psram_BURST,psram_OVERTIMER);
    tls_os_time_delay (HZ/10);
    if(d_psram_check())
      {
      xMessageBufferSize=psram_BufferSize;
      ucStorageBuffer = dram_heap_malloc (xMessageBufferSize+1);
      }
      else
      {
      xMessageBufferSize=no_psram_BufferSize;
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
      VS1053_WEB_RADIO_buf_chunk_free = 0;
      while (
          (nRetCode == HTTP_CLIENT_SUCCESS || nRetCode != HTTP_CLIENT_EOS)
          //&& my_sost == VS1053_HW_INIT
          )
        {

          u32 u_fur=tls_os_get_time();
          nSize = HTTP_CLIENT_BUFFER_SIZE;
          size_t freeSize = xMessageBufferSpacesAvailable (xMessageBuffer);
          //ptintf ("%d ", freeSize);
          if (freeSize < (nSize + 4))// || VS1053_WEB_RADIO_buf_chunk_free>DF_VS1053_WEB_RADIO_buf_chunk_top)// || i_cnt++>4096)
            {
            printf ("PreLoad buf, freeSize=%d\r\n", freeSize);
            break;
            }
          nRetCode = HTTPClientReadData (pHTTP, Buffer, nSize,
                                         u16_connect_timeout_sec, &nSize);
          u32 u_cur = tls_os_get_time ();
          //printf ("u_cur=%d, u_fur=%d \n",u_cur, u_fur );
          if((u_cur-u_fur-3)>0 && (u_cur-u_fur-3)<HZ)
             tls_os_time_delay (u_cur-u_fur-3);
          else 
             tls_os_time_delay (0);

          if (nRetCode == HTTP_CLIENT_SUCCESS)
            {
              xMessageBufferSend (xMessageBuffer, Buffer, nSize,
                                  portMAX_DELAY);
              if(load_buffer_debug)
                  printf ("f");
              VS1053_WEB_RADIO_buf_chunk_free++;
              VS1053_WEB_RADIO_nTotal += nSize;
              if (VS1053_WEB_RADIO_nTotal > 512)
                 tls_watchdog_clr ();
            }
        }
      if(VS1053_WEB_RADIO_buf_chunk_free>VS1053_WEB_RADIO_buf_chunk_total)
        VS1053_WEB_RADIO_buf_chunk_total=VS1053_WEB_RADIO_buf_chunk_free;
      //u32 u_cur = tls_os_get_time ();
      //printf ("u_cur=%d, u_fur=%d \n",u_cur, u_fur );
      //while ( (tls_os_get_time () - u_cur) < (u_cur-u_fur)  && my_sost == VS1053_HW_INIT )
      //  { //
      //    tls_os_time_delay (0);
      //    tls_watchdog_clr ();
      //  }
      printf ("start play\n");

      if (my_sost != VS1053_QUERY_TO_STOP)
        my_sost = VS1053_PLAY;
      tls_os_time_delay (1000);

      VS1053_WEB_RADIO_nTotal = 0;
      // Get the data until we get an error or end of stream code
      while (
          (nRetCode == HTTP_CLIENT_SUCCESS || nRetCode != HTTP_CLIENT_EOS)
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
          u32 u_cur = tls_os_get_time ();
          //printf ("u_cur=%d, u_fur=%d \n",u_cur, u_fur );
          if((u_cur-u_fur-3)>0 && (u_cur-u_fur-3)<HZ)
             tls_os_time_delay (u_cur-u_fur-3);
          else 
             tls_os_time_delay (0);



          //ждем свободное место в буфере, и заполняем его
          while (my_sost == VS1053_PLAY)
            {
              size_t freeSize = xMessageBufferSpacesAvailable (xMessageBuffer);
              if (freeSize > (nSize + 4) && VS1053_WEB_RADIO_buf_chunk_free<=DF_VS1053_WEB_RADIO_buf_chunk_top)
                {
                  xMessageBufferSend (xMessageBuffer, Buffer, nSize,
                                      portMAX_DELAY);
                  //printf (" %d ", freeSize);
                  if(load_buffer_debug)
                     printf ("+");
                  VS1053_WEB_RADIO_buf_chunk_free++;
                  if(VS1053_WEB_RADIO_buf_chunk_free>VS1053_WEB_RADIO_buf_chunk_total)
                    VS1053_WEB_RADIO_buf_chunk_total=VS1053_WEB_RADIO_buf_chunk_free;
                  break;
                }
              tls_os_time_delay (1);
            }

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