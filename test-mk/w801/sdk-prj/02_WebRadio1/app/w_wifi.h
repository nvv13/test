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

  int scan_demo (void);

  //int scan_format2_demo (void);
  u8 scan_format2_demo (const char ** aWIFI,const u8 u8_WIFI_ARR_COUNT);

  /*specified Scan demo*/
  int scan_specified_demo (u8 *ssid, u8 *mac, u32 chan, u32 scan_type,
                           u32 min_interval, u32 max_interval);

#ifdef __cplusplus
}
#endif

#endif /* W_WIFI_H */
