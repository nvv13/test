
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HTTPClient.h"
#include "wm_type_def.h"

#include "my_recognize.h"

static const char *c_stationuuid = "\"stationuuid\":\"";
static u16 i_POS_stationuuid = 0;
static u16 i_LOAD_stationuuid = 0;
static char s_stationuuid[50];

static const char *c_name = "\"name\":\"";
static u16 i_POS_name = 0;
static u16 i_LOAD_name = 0;
static char s_name[150];

static const char *c_url_resolved = "\"url_resolved\":\"";
static u16 i_POS_url_resolved = 0;
static u16 i_LOAD_url_resolved = 0;
static char s_url_resolved[250];

static const char *c_tags = "\"tags\":\"";
static u16 i_POS_tags = 0;
static u16 i_LOAD_tags = 0;
static char s_tags[50];

static const char *c_country = "\"country\":\"";
static u16 i_POS_country = 0;
static u16 i_LOAD_country = 0;
static char s_country[50];

void
my_recognize_http_reset (void)
{
  i_POS_stationuuid = 0;
  i_LOAD_stationuuid = 0;
  i_POS_name = 0;
  i_LOAD_name = 0;
  i_POS_url_resolved = 0;
  i_LOAD_url_resolved = 0;
  i_POS_tags = 0;
  i_LOAD_tags = 0;
  i_POS_country = 0;
  i_LOAD_country = 0;
}

void
my_recognize_http_error (void)
{
  my_recognize_http_reset ();
}

static void
load_field (const char ch, const char *c_find, u16 *i_pos_find, char *s_field,
            u16 *i_pos_field, const u16 i_len_field)
{
  if ((*i_pos_find) < strlen (c_find) && ch == c_find[(*i_pos_find)])
    (*i_pos_find)++;
  else
    {
      if ((*i_pos_find) < strlen (c_find))
        (*i_pos_find) = 0;
      else
        {
          if ((*i_pos_field) < (i_len_field - 1))
            {
              s_field[(*i_pos_field)] = ch;
              if ((*i_pos_field) == (i_len_field - 2) || ch == '"')
                {
                  s_field[(*i_pos_field)] = 0;
                  (*i_pos_field) = i_len_field;
                  printf ("%s=\"%s\"\n", c_find, s_field);
                }
              (*i_pos_field)++;
            }
        }
    }
}

void
my_recognize_http (const char *recvbuf, int i_len)
{
  // printf ("%s\n",recvbuf);
  for (int iInd = 0; iInd < i_len; iInd++)
    {
      char ch = *(recvbuf + iInd);
      load_field (ch, c_stationuuid, &i_POS_stationuuid, s_stationuuid,
                  &i_LOAD_stationuuid, sizeof (s_stationuuid));
      load_field (ch, c_name, &i_POS_name, s_name, &i_LOAD_name,
                  sizeof (s_name));
      load_field (ch, c_url_resolved, &i_POS_url_resolved, s_url_resolved,
                  &i_LOAD_url_resolved, sizeof (s_url_resolved));
      load_field (ch, c_tags, &i_POS_tags, s_tags, &i_LOAD_tags,
                  sizeof (s_tags));
      load_field (ch, c_country, &i_POS_country, s_country, &i_LOAD_country,
                  sizeof (s_country));
    }
}

#define HTTP_CLIENT_BUFFER_SIZE 1024

u8 RemoteIp[4]
    = { 192, 168, 1, 100 }; // Remote server's IP when test http function

u32
http_snd_req (HTTPParameters ClientParams, HTTP_VERB verb, char *pSndData,
              u8 parseXmlJson)
{
  int nRetCode;
  u32 nSize, nTotal = 0;
  char *Buffer = NULL;
  HTTP_SESSION_HANDLE pHTTP;
  u32 nSndDataLen;
  do
    {
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
          my_recognize_http_error ();
          nRetCode = HTTP_CLIENT_ERROR_INVALID_HANDLE;
          break;
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
          my_recognize_http_error ();
          break;
        }
#if TLS_CONFIG_HTTP_CLIENT_AUTH
        // Set authentication
//        if(ClientParams.AuthType != AuthSchemaNone)
//        {
//            if((nRetCode = HTTPClientSetAuth(pHTTP, ClientParams.AuthType,
//            NULL)) != HTTP_CLIENT_SUCCESS)
//            {
//                my_recognize_http_error ();
//                break;
//            }
//
//            // Set authentication
//            if((nRetCode = HTTPClientSetCredentials(pHTTP,
//            ClientParams.UserName, ClientParams.Password)) !=
//            HTTP_CLIENT_SUCCESS)
//            {
//                my_recognize_http_error ();
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
//                my_recognize_http_error ();
//                break;
//            }
//        }
#endif // TLS_CONFIG_HTTP_CLIENT_PROXY
      if ((nRetCode = HTTPClientSendRequest (
               pHTTP, ClientParams.Uri, pSndData, nSndDataLen,
               verb == VerbPost || verb == VerbPut, 0, 0))
          != HTTP_CLIENT_SUCCESS)
        {
          my_recognize_http_error ();
          break;
        }
      // Retrieve the the headers and analyze them
      if ((nRetCode = HTTPClientRecvResponse (pHTTP, 30))
          != HTTP_CLIENT_SUCCESS)
        {
          my_recognize_http_error ();
          break;
        }
      printf ("Start to receive data from remote server...\r\n");

      // Get the data until we get an error or end of stream code
      while (nRetCode == HTTP_CLIENT_SUCCESS || nRetCode != HTTP_CLIENT_EOS)
        {
          // Set the size of our buffer
          nSize = HTTP_CLIENT_BUFFER_SIZE;
          // Get the data
          nRetCode = HTTPClientReadData (pHTTP, Buffer, nSize, 300, &nSize);
          if (nRetCode != HTTP_CLIENT_SUCCESS && nRetCode != HTTP_CLIENT_EOS)
            break;
          // printf("%s", Buffer);
          my_recognize_http (Buffer, nSize);
          nTotal += nSize;
        }
    }
  while (0); // Run only once
  tls_mem_free (Buffer);

  if (pHTTP)
    HTTPClientCloseRequest (&pHTTP);
  if (ClientParams.Verbose == TRUE)
    {
      printf ("\n\nHTTP Client terminated %d (got %d b)\n\n", nRetCode,
              nTotal);
    }
  return nRetCode;
}

u32
http_get (HTTPParameters ClientParams)
{
  return http_snd_req (ClientParams, VerbGet, NULL, 0);
}

int
http_get_demo (char *buf)
{
  HTTPParameters httpParams;

  memset (&httpParams, 0, sizeof (HTTPParameters));
  httpParams.Uri = (char *)tls_mem_alloc (128);
  if (httpParams.Uri == NULL)
    {
      printf ("malloc error.\n");
      return WM_FAILED;
    }
  memset (httpParams.Uri, 0, 128);
  sprintf (httpParams.Uri, "%s", buf);
  httpParams.Verbose = TRUE;
  printf ("Location: %s\n", httpParams.Uri);
  http_get (httpParams);
  tls_mem_free (httpParams.Uri);

  return WM_SUCCESS;
}
