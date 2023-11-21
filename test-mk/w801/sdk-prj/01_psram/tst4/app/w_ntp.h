#ifndef W_NTP_H
#define W_NTP_H

//#include <string.h>
//#include <stdio.h>
//#include <stdlib.h>
#include "wm_cpu.h"
#include "wm_type_def.h"
//#include "wm_uart.h"
//#include "wm_gpio.h"
//#include "wm_hostspi.h"
//#include "wm_socket.h"
//#include "wm_sockets.h"
#include "wm_wifi.h"
//#include "wm_hspi.h"
//#include "wm_pwm.h"
//#include "wm_params.h"
#include "wm_netif.h"
#include "wm_osal.h"
//#include "wm_efuse.h"
#include "wm_mem.h"
//#include "wm_regs.h"

//#ifdef __cplusplus
// extern "C" {
//#endif

int ntp_demo (void);
int ntp_set_server_demo (char *ipaddr1, char *ipaddr2, char *ipaddr3);

//#ifdef __cplusplus
// extern }
//#endif

#endif /* W_NTP_H */
