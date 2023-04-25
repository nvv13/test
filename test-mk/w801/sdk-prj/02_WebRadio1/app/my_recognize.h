#ifndef MY_RECOGNIZE_H
#define MY_RECOGNIZE_H

void my_recognize_http_reset (void);
void my_recognize_http_error (void);
void my_recognize_http (const char *recvbuf, int i_len);

int http_get_demo (char *buf);

#endif /* MY_RECOGNIZE_H */