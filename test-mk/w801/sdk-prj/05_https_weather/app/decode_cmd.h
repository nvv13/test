#ifndef DECODE_CMD_H
#define DECODE_CMD_H

#define OTA_PATH_FILE "http://192.168.1.69/ota/weather/w800_ota.img"

int fast_decode_cmd(char* sock_rx, int len);

#endif /* DECODE_CMD_H */
