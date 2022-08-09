#ifndef DECODE_CMD_H
#define DECODE_CMD_H

#define VERSION_FLASH "0.8"

#define OTA_PATH_FILE "http://192.168.1.69/ota/weather/w800_ota.img"

#define GL_MODE_WEATHER       1
#define GL_MODE_WEATHER_CLOCK 2
#define GL_MODE_CLOCK         3

#define MEM_CELL_FROM_LIGTH_LEVEL 0
#define MEM_CELL_FROM_GL_MODE     1

int fast_decode_cmd(char* sock_rx, int len);

#endif /* DECODE_CMD_H */
