#ifndef DECODE_CMD_H
#define DECODE_CMD_H

#define VERSION_FLASH "0.8"

#define OTA_PATH_FILE "http://192.168.1.69/ota/rgb_led/w800_ota.img"
#include "wm_http_fwup.h"
extern int t_http_fwup(char *url);


#define MEM_CELL_FROM_LIGTH_LEVEL 0

int fast_decode_cmd(char* sock_rx, int len);

#endif /* DECODE_CMD_H */
