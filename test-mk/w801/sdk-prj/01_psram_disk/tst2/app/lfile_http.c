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

//#include <FreeRTOS.h>
//#include <message_buffer.h>
#include "ff.h"

#include "HTTPClient.h"
#include "wm_type_def.h"
#include "wm_watchdog.h"

#define HTTP_CLIENT_BUFFER_SIZE 1024

static u32
http_snd_req (HTTPParameters ClientParams, HTTP_VERB verb, char *pSndData,
              u8 parseXmlJson, FIL *pfnew)
{
  int nRetCode;
  u32 nSize = 0;
  char *Buffer = NULL;
  HTTP_SESSION_HANDLE pHTTP;
  u32 nSndDataLen;

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

  P_HTTP_SESSION pHTTPSession = (P_HTTP_SESSION)pHTTP;
  pHTTPSession->HttpFlags
      = pHTTPSession->HttpFlags | HTTP_CLIENT_FLAG_KEEP_ALIVE;

  int i_ByteTotal = 0;
  u32 content_length = 0;
  do
    {
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

      char token[32];
      UINT32 size = 32;
      if ((nRetCode = HTTPClientFindFirstHeader (pHTTP, "content-length",
                                                 (CHAR *)token, &size))
          != HTTP_CLIENT_SUCCESS)
        {
          HTTPClientFindCloseHeader (pHTTP);
        }
      else
        {
          HTTPClientFindCloseHeader (pHTTP);
          content_length = atol (strstr (token, ":") + 1);
          printf ("content_length: %d\n", content_length);
        }

      tls_os_time_delay (HZ / 3);

      u16 u16_connect_timeout_sec = 300;
      if (strstr (ClientParams.Uri, "https") != NULL)
        u16_connect_timeout_sec = 600;

      printf ("Start to receive data from remote server\r\n");
      tls_os_time_delay (HZ);

      // Get the data until we get an error or end of stream code
      while (nRetCode == HTTP_CLIENT_SUCCESS || nRetCode != HTTP_CLIENT_EOS)
        {
          // Set the size of our buffer
          nSize = HTTP_CLIENT_BUFFER_SIZE;
          // Get the data
          nRetCode = HTTPClientReadData (pHTTP, Buffer, nSize,
                                         u16_connect_timeout_sec, &nSize);
          if (nRetCode != HTTP_CLIENT_SUCCESS && nRetCode != HTTP_CLIENT_EOS)
            {
              printf ("HTTPClientReadData nRetCode:%d\r\n", nRetCode);
              break;
            }

          FRESULT res_sd = f_write (pfnew, Buffer, nSize, &nSize);
          if (res_sd != FR_OK)
            {
              nRetCode = res_sd;
              printf ("f_write failed! error code:%d\r\n", res_sd);
              break;
            }

          i_ByteTotal += nSize;
          printf ("download bytes: %d\r", i_ByteTotal);
          if (i_ByteTotal == content_length)
            break;
        }
    }
  while (1 == 0);
  tls_mem_free (Buffer);
  printf (" download bytes: %d\r\n", i_ByteTotal);

  if (i_ByteTotal == content_length || nRetCode == HTTP_CLIENT_EOS)
    nRetCode = FR_OK;

  if (pHTTP)
    HTTPClientCloseRequest (&pHTTP);
  return nRetCode;
}

FRESULT
download_file_http (const char *Uri, FIL *pfnew)
{

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
  int res_sd = http_snd_req (httpParams, VerbGet, NULL, 0, pfnew);
  tls_mem_free (httpParams.Uri);

  return res_sd;
}