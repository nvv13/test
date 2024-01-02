#ifndef W_WIFI_H
#define W_WIFI_H

#ifdef __cplusplus
extern "C"
{
#endif

//#include "wm_type_def.h"
//#include "wm_wifi.h"
#include "wm_netif.h"
  //#include "wm_mem.h"

  int wifi_connect (const char *ssid, const char *pwd);

  extern u8 volatile u8_wifi_state;

#ifdef __cplusplus
}
#endif

#endif /* W_WIFI_H */
