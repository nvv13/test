#ifndef MY_RECOGNIZE_H
#define MY_RECOGNIZE_H

#ifdef __cplusplus
extern "C"
{
#endif

void my_recognize_http_reset (void);

char * my_recognize_ret_stationuuid (void);
char * my_recognize_ret_name (void);
char * my_recognize_ret_url_resolved (void);
char * my_recognize_ret_tags (void);
char * my_recognize_ret_country (void);
char * my_recognize_ret_codec (void);
char * my_recognize_ret_bitrate (void);


int http_get_web_station_by_random (void);
int http_get_web_station_by_stationuuid (char * in_stationuuid);

#ifdef __cplusplus
}
#endif

#endif /* MY_RECOGNIZE_H */