/*

$ wget 'http://all.api.radio-browser.info/json/stations/bytag/classic?codec=mp3&limit=1&order=random' -O 1.json

[{
"changeuuid":"55dd77ed-d8d3-4333-9f55-ac3a3f863922",
"stationuuid":"962560b0-0601-11e8-ae97-52543be04c81",
"serveruuid":null,
"name":"WPSU 91.5 Penn State University - State College, PA",
"url":"http://wpsu-ice.streamguys1.com/wpsu1",
"url_resolved":"http://wpsu-ice.streamguys1.com/wpsu1",
"homepage":"http://wpsu.org/",
"favicon":"http://wpsu.org/apple-touch-icon-120x120.png",
"tags":"classical,npr,penn state,pri,public radio,state college",
"country":"The United States Of America",
"countrycode":"US",
"iso_3166_2":null,
"state":"Pennsylvania",
"language":"english",
"languagecodes":"",
"votes":36,
"lastchangetime":"2022-11-28 08:09:30",
"lastchangetime_iso8601":"2022-11-28T08:09:30Z",
"codec":"MP3",
"bitrate":96,
"hls":0,
"lastcheckok":1,
"lastchecktime":"2023-04-28 10:42:26",
"lastchecktime_iso8601":"2023-04-28T10:42:26Z",
"lastcheckoktime":"2023-04-28 10:42:26",
"lastcheckoktime_iso8601":"2023-04-28T10:42:26Z",
"lastlocalchecktime":"",
"lastlocalchecktime_iso8601":null,
"clicktimestamp":"2023-04-24 23:08:58",
"clicktimestamp_iso8601":"2023-04-24T23:08:58Z",
"clickcount":4,
"clicktrend":0,
"ssl_error":0,
"geo_lat":null,
"geo_long":null,
"has_extended_info":false
}]



*/


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

static const char *c_codec = "\"codec\":\"";
static u16 i_POS_codec = 0;
static u16 i_LOAD_codec = 0;
static char s_codec[10];

static const char *c_bitrate = "\"bitrate\":";
static u16 i_POS_bitrate = 0;
static u16 i_LOAD_bitrate = 0;
static char s_bitrate[10];


char * my_recognize_ret_stationuuid (void)
{
  return s_stationuuid;
}

char * my_recognize_ret_name (void)
{
  return s_name;
}

char *
my_recognize_ret_url_resolved (void)
{
  return s_url_resolved;
}

char * my_recognize_ret_country (void)
{
  return s_country;
}

char * my_recognize_ret_tags (void)
{
  return s_tags;
}

char * my_recognize_ret_codec (void)
{
  return s_codec;
}

char * my_recognize_ret_bitrate (void)
{
  return s_bitrate;
}

void
my_recognize_http_reset (void)
{
  i_POS_stationuuid = 0;
  i_LOAD_stationuuid = 0;
  s_stationuuid[0]=0;
  i_POS_name = 0;
  i_LOAD_name = 0;
  s_name[0]=0;
  i_POS_url_resolved = 0;
  i_LOAD_url_resolved = 0;
  s_url_resolved[0]=0;
  i_POS_tags = 0;
  i_LOAD_tags = 0;
  s_tags[0]=0;
  i_POS_country = 0;
  i_LOAD_country = 0;
  s_country[0]=0;
  i_POS_codec = 0;
  i_LOAD_codec = 0;
  s_codec[0]=0;
  i_POS_bitrate = 0;
  i_LOAD_bitrate = 0;
  s_bitrate[0]=0;
}

static void
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

static void
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
      load_field (ch, c_codec, &i_POS_codec, s_codec, &i_LOAD_codec,
                  sizeof (s_codec));
      load_field (ch, c_bitrate, &i_POS_bitrate, s_bitrate, &i_LOAD_bitrate,
                  sizeof (s_bitrate));
    }
}

#define HTTP_CLIENT_BUFFER_SIZE 512

static u32
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
          nRetCode = HTTPClientReadData (pHTTP, Buffer, nSize, 500, &nSize);
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

static u32
http_get (HTTPParameters ClientParams)
{
  return http_snd_req (ClientParams, VerbGet, NULL, 0);
}


int http_get_web_station_by_stationuuid (char * in_stationuuid)
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
  sprintf (httpParams.Uri, "http://all.api.radio-browser.info/json/stations/byuuid/%s", in_stationuuid);
  httpParams.Verbose = TRUE;
  printf ("Location: %s\n", httpParams.Uri);
  http_get (httpParams);
  tls_mem_free (httpParams.Uri);
  return WM_SUCCESS;
}

static u8 u8_ch_st_uri=0;

int
http_get_web_station_by_random (void)
{
  HTTPParameters httpParams;
  memset (&httpParams, 0, sizeof (HTTPParameters));
  switch(u8_ch_st_uri)
  {
  case 0 : httpParams.Uri = "http://all.api.radio-browser.info/json/stations/bycodec/mp3?limit=1&order=random";break;
  case 1 : httpParams.Uri = "http://all.api.radio-browser.info/json/stations/bytag/rock?codec=mp3&limit=1&order=random";break;
  case 2 : httpParams.Uri = "http://all.api.radio-browser.info/json/stations/bytag/classic?codec=mp3&limit=1&order=random";break;
  case 3 : httpParams.Uri = "http://all.api.radio-browser.info/json/stations/bytag/trance?codec=mp3&limit=1&order=random";break;
  case 4 : httpParams.Uri = "http://all.api.radio-browser.info/json/stations/bytag/pop?codec=mp3&limit=1&order=random";break;
  case 5 : httpParams.Uri = "http://all.api.radio-browser.info/json/stations/bycountry/rus?codec=mp3&limit=1&order=random";break;
  }
  if(++u8_ch_st_uri>5)u8_ch_st_uri=0;
  //
  httpParams.Verbose = TRUE;
  printf ("Location: %s\n", httpParams.Uri);
  http_get (httpParams);
  tls_mem_free (httpParams.Uri);
  return WM_SUCCESS;
}
