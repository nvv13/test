#ifndef W_WIFI_H
#define W_WIFI_H

//#include <string.h>
//#include <stdio.h>
//#include <stdlib.h>
#include "wm_type_def.h"
//#include "wm_cpu.h"
//#include "wm_uart.h"
//#include "wm_gpio.h"
//#include "wm_hostspi.h"
//#include "wm_socket.h"
//#include "wm_sockets.h"
#include "wm_wifi.h"
//#include "wm_hspi.h"
//#include "wm_pwm.h"
//#include "wm_params.h"
//#include "wm_osal.h"
#include "wm_netif.h"
//#include "wm_efuse.h"
#include "wm_mem.h"
//#include "wm_regs.h"


#define DEMO_CONNECT_NET 1

//#ifdef __cplusplus
//extern "C" {
//#endif


int demo_connect_net(char *ssid, char *pwd);

int create_socket_server(int port);

//#ifdef __cplusplus
//extern }
//#endif


#endif /* W_WIFI_H */
