/*

$ wget
'http://all.api.radio-browser.info/json/stations/bytag/classic?codec=mp3&limit=1&order=random'
-O 1.json

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

#include "my_recognize.h"
//#include "n_utf8_to_win1251.h"

static u8 u8_index = 0;

static const char *c_stationuuid = "\"stationuuid\":\"";
static u16 i_POS_stationuuid = 0;
static u16 i_LOAD_stationuuid = 0;
static char s_stationuuid[MAX_INDEX_LOAD_FIND][37];

static const char *c_name = "\"name\":\"";
static u16 i_POS_name = 0;
static u16 i_LOAD_name = 0;
static char s_name[MAX_INDEX_LOAD_FIND][32];

static const char *c_url_resolved = "\"url_resolved\":\"";
static u16 i_POS_url_resolved = 0;
static u16 i_LOAD_url_resolved = 0;
static char s_url_resolved[MAX_INDEX_LOAD_FIND][200];

static const char *c_tags = "\"tags\":\"";
static u16 i_POS_tags = 0;
static u16 i_LOAD_tags = 0;
static char s_tags[MAX_INDEX_LOAD_FIND][32];

static const char *c_country = "\"country\":\"";
static u16 i_POS_country = 0;
static u16 i_LOAD_country = 0;
static char s_country[MAX_INDEX_LOAD_FIND][32];

static const char *c_codec = "\"codec\":\"";
static u16 i_POS_codec = 0;
static u16 i_LOAD_codec = 0;
static char s_codec[MAX_INDEX_LOAD_FIND][10];

static const char *c_bitrate = "\"bitrate\":";
static u16 i_POS_bitrate = 0;
static u16 i_LOAD_bitrate = 0;
static char s_bitrate[MAX_INDEX_LOAD_FIND][5];

char *
my_recognize_ret_stationuuid (u8 index)
{
  if (index > u8_index)
    index = u8_index;
  return s_stationuuid[index];
}

char *
my_recognize_ret_name (u8 index)
{
  if (index > u8_index)
    index = u8_index;
  if (strlen (s_name[index]) == 0)
    {
      return s_tags[index];
    }
  return s_name[index];
}

char *
my_recognize_ret_url_resolved (u8 index)
{
  if (index > u8_index)
    index = u8_index;
  return s_url_resolved[index];
}

char *
my_recognize_ret_country (u8 index)
{
  if (index > u8_index)
    index = u8_index;
  return s_country[index];
}

char *
my_recognize_ret_tags (u8 index)
{
  if (index > u8_index)
    index = u8_index;
  return s_tags[index];
}

char *
my_recognize_ret_codec (u8 index)
{
  if (index > u8_index)
    index = u8_index;
  return s_codec[index];
}

char *
my_recognize_ret_bitrate (u8 index)
{
  if (index > u8_index)
    index = u8_index;
  return s_bitrate[index];
}

bool
my_recognize_ret_https (u8 index)
{
  if (index > u8_index)
    index = u8_index;
  return (strstr (s_url_resolved[index], "https:") != NULL);
}

void
my_recognize_http_reset (void)
{
  u8_index = 0;
  //
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
  i_POS_codec = 0;
  i_LOAD_codec = 0;
  i_POS_bitrate = 0;
  i_LOAD_bitrate = 0;
  for (u8 ind = 0; ind < MAX_INDEX_LOAD_FIND; ind++)
    {
      s_stationuuid[ind][0] = 0;
      s_name[ind][0] = 0;
      s_url_resolved[ind][0] = 0;
      s_tags[ind][0] = 0;
      s_country[ind][0] = 0;
      s_codec[ind][0] = 0;
      s_bitrate[ind][0] = 0;
    }
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
      load_field (ch, c_stationuuid, &i_POS_stationuuid,
                  s_stationuuid[u8_index], &i_LOAD_stationuuid,
                  sizeof (s_stationuuid[u8_index]));
      load_field (ch, c_name, &i_POS_name, s_name[u8_index], &i_LOAD_name,
                  sizeof (s_name[u8_index]));
      load_field (ch, c_url_resolved, &i_POS_url_resolved,
                  s_url_resolved[u8_index], &i_LOAD_url_resolved,
                  sizeof (s_url_resolved[u8_index]));
      load_field (ch, c_tags, &i_POS_tags, s_tags[u8_index], &i_LOAD_tags,
                  sizeof (s_tags[u8_index]));
      load_field (ch, c_country, &i_POS_country, s_country[u8_index],
                  &i_LOAD_country, sizeof (s_country[u8_index]));
      load_field (ch, c_codec, &i_POS_codec, s_codec[u8_index], &i_LOAD_codec,
                  sizeof (s_codec[u8_index]));
      load_field (ch, c_bitrate, &i_POS_bitrate, s_bitrate[u8_index],
                  &i_LOAD_bitrate, sizeof (s_bitrate[u8_index]));
      if (ch == '}' && u8_index < MAX_INDEX_LOAD_FIND - 1)
        u8_index++;
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
  my_recognize_http_reset ();
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

int
http_get_web_station_by_stationuuid (char *in_stationuuid)
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
  sprintf (httpParams.Uri,
           "http://all.api.radio-browser.info/json/stations/byuuid/%s",
           in_stationuuid);
  httpParams.Verbose = TRUE;
  printf ("Location: %s\n", httpParams.Uri);
  http_get (httpParams);
  tls_mem_free (httpParams.Uri);
  return WM_SUCCESS;
}

static u8 u8_ch_st_uri = 0;
static u8 u8_ch_st_ord = 0;

int
http_get_web_station_by_random (void)
{
  HTTPParameters httpParams;
  memset (&httpParams, 0, sizeof (HTTPParameters));

  char *s_tag;
  switch (u8_ch_st_uri)
    {
    case 0:
      s_tag = "piano";
      break;
    case 1:
      s_tag = "trance";
      break;
    case 2:
      s_tag = "rock";
      break;
    case 3:
      s_tag = "classic";
      break;
    case 4:
      s_tag = "organ";
      break;
    case 5:
      s_tag = "pop";
      break;
    case 6:
      s_tag = "guitar";
      break;
    case 7:
      s_tag = "j-pop";
      break;
    case 8:
      s_tag = "k-pop";
      break;
    case 9:
      s_tag = "chopin";
      break;
    default:
      s_tag = "top";
    }
  if (++u8_ch_st_uri > 9)
    {
      u8_ch_st_uri = 0;
      u8_ch_st_ord = ~u8_ch_st_ord;
    }
  char *s_order;
  if (u8_ch_st_ord == 0)
    s_order = "order=random&reverse=true";
  else
    s_order = "order=random";
  //
  httpParams.Uri = (char *)tls_mem_alloc (128);
  if (httpParams.Uri == NULL)
    {
      printf ("malloc error.\n");
      return WM_FAILED;
    }
  memset (httpParams.Uri, 0, 128);
  sprintf (
      httpParams.Uri,
      "http://all.api.radio-browser.info/json/stations/bytag/%s?limit=%d&%s",
      s_tag, MAX_INDEX_LOAD_FIND, s_order);
  httpParams.Verbose = TRUE;
  printf ("Location: %s\n", httpParams.Uri);
  http_get (httpParams);
  tls_mem_free (httpParams.Uri);
  return WM_SUCCESS;
}
