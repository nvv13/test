#ifndef DECODE_CMD_H
#define DECODE_CMD_H

#define VERSION_FLASH "0.01"

#define OTA_PATH_FILE "http://192.168.1.1:8088/ota/audio1/w800_ota.img"
#include "wm_http_fwup.h"
extern int t_http_fwup (char *url);

int fast_decode_cmd (char *sock_rx, int len);

#endif /* DECODE_CMD_H */
