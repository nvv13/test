/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: pin PB1, freq = 5,714309 MHz CPU_CLK_240M
 *
 * Date : 2022-05-20
 *****************************************************************************/

#define DEMO_BT CFG_ON
#define TLS_CONFIG_BR_EDR CFG_ON


#include "wm_type_def.h"

//#include "wm_cpu.h"
//#include "wm_gpio.h"
#include "wm_osal.h"
//#include "wm_regs.h"
#include <stdio.h>

#include "wm_uart.h"

#include "wm_bt_def.h"

#include "wm_bt_spp.h"


//#include "wm_ble_uart_if.h"

#if DEMO_BT
 extern int demo_bt_enable (void *, ...);
// extern int demo_bt_destroy (void *, ...);
//#if (TLS_CONFIG_BLE == CFG_ON)
// extern int demo_ble_server_on (void *, ...);
// extern int demo_ble_server_off (void *, ...);
// extern int demo_ble_client_on (void *, ...);
// extern int demo_ble_client_off (void *, ...);
// extern int demo_ble_scan (void *, ...);
// extern int demo_ble_adv (void *, ...);
//#endif

//#if (TLS_CONFIG_BR_EDR == CFG_ON)
// extern int demo_bt_app_on (void *, ...);
// extern int demo_bt_app_off (void *, ...);
//#endif

#endif

void
UserMain (void)
{

  printf ("user task\n");

  demo_bt_enable (0);
  printf ("demo_bt_enable\n");
  tls_os_time_delay (HZ * 5);

  //  demo_ble_server_on (0);
  //  printf ("demo_ble_server_on\n");
/*
  uint8_t uart_id = 2;
  tls_uart_options_t opt;

  opt.baudrate = 9600;
  opt.paritytype = TLS_UART_PMODE_DISABLED;
  opt.stopbits = TLS_UART_ONE_STOPBITS;
  opt.charlength = TLS_UART_CHSIZE_8BIT;
  opt.flow_ctrl = TLS_UART_FLOW_CTRL_NONE;

  int i_Res = tls_ble_uart_init (BLE_UART_SERVER_MODE, uart_id, &opt);
*/

  tls_bt_status_t tt=tls_bt_spp_start_server(WM_SPP_SEC_NONE, WM_SPP_ROLE_SERVER , 0, "WM_12345");
  printf ("tls_bt_spp_start_server tls_bt_status_t=%d\n",(int)tt);


  return;
}
