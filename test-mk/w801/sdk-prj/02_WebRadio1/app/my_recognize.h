#ifndef MY_RECOGNIZE_H
#define MY_RECOGNIZE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "wm_type_def.h"

#define MAX_INDEX_LOAD_FIND 8

  void my_recognize_http_reset (void);

  char *my_recognize_ret_stationuuid (u8 index);
  char *my_recognize_ret_name (u8 index);
  char *my_recognize_ret_url_resolved (u8 index);
  char *my_recognize_ret_tags (u8 index);
  char *my_recognize_ret_country (u8 index);
  char *my_recognize_ret_codec (u8 index);
  char *my_recognize_ret_bitrate (u8 index);
  bool my_recognize_ret_https (u8 index);

  int http_get_web_station_by_random (void);
  int http_get_web_station_by_stationuuid (char *in_stationuuid);

#ifdef __cplusplus
}
#endif

#endif /* MY_RECOGNIZE_H */