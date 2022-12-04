/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: pin PB1, freq = 5,714309 MHz CPU_CLK_240M
 *
 * Date : 2022-05-20
 *****************************************************************************/

#include "wm_type_def.h"

//#include "wm_cpu.h"
//#include "wm_gpio.h"
#include "wm_osal.h"
//#include "wm_regs.h"
#include <stdio.h>

//#if DEMO_BT
extern int demo_bt_enable (void *, ...);
extern int demo_bt_destroy (void *, ...);
//#if (TLS_CONFIG_BLE == CFG_ON)
extern int demo_ble_server_on (void *, ...);
extern int demo_ble_server_off (void *, ...);
extern int demo_ble_client_on (void *, ...);
extern int demo_ble_client_off (void *, ...);

extern int demo_ble_scan (void *, ...);
extern int demo_ble_adv (void *, ...);
//#endif

//#if (TLS_CONFIG_BR_EDR == CFG_ON)
//extern int demo_bt_app_on (void *, ...);
//extern int demo_bt_app_off (void *, ...);
//#endif

//#endif

void
UserMain (void)
{

  printf ("user task\n");

  demo_bt_enable (0);
  printf ("demo_bt_enable\n");
  tls_os_time_delay (HZ * 5);

  demo_ble_server_on (0);
  printf ("demo_ble_server_on\n");

  return;
}

