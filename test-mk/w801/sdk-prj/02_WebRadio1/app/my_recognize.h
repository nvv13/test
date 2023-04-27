#ifndef MY_RECOGNIZE_H
#define MY_RECOGNIZE_H

void my_recognize_http_reset (void);
void my_recognize_http_error (void);
void my_recognize_http (const char *recvbuf, int i_len);

char * my_recognize_ret_stationuuid (void);
char * my_recognize_ret_name (void);
char * my_recognize_ret_url_resolved (void);
char * my_recognize_ret_tags (void);
char * my_recognize_ret_country (void);


int http_get_web_station_by_random (void);
int http_get_web_station_by_stationuuid (char * in_stationuuid);

#endif /* MY_RECOGNIZE_H */