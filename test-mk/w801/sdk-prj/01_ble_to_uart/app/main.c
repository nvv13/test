/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: pin PB1, freq = 5,714309 MHz CPU_CLK_240M
 *
 * Date : 2022-05-20
 *****************************************************************************/

#include <stdio.h>
#include <string.h>

#include "wm_type_def.h"

#include "wm_gpio_afsel.h"

#include "../src/app/bleapp/wm_ble_uart_if.h"

extern int demo_bt_enable (void);

void
UserMain (void)
{

  printf ("user task\n");

  demo_bt_enable ();
  printf ("demo_bt_enable... delay 5 sec\n");
  tls_os_time_delay (HZ * 5);

  uint8_t uart_no = TLS_UART_1; // TLS_UART_0 (USB)
                                // TLS_UART_1 .. TLS_UART_5

  tls_uart_options_t opt;
  opt.baudrate = UART_BAUDRATE_B115200;
  opt.charlength = TLS_UART_CHSIZE_8BIT;
  opt.flow_ctrl = TLS_UART_FLOW_CTRL_NONE;
  opt.paritytype = TLS_UART_PMODE_DISABLED;
  opt.stopbits = TLS_UART_ONE_STOPBITS;

  // Select the pin to be used and the specific alternate function
  /* UART1_RX-PB07  UART1_TX-PB06 */

  /**
   * @brief  config the pins used for uart1 tx
   * @param  io_name: config uart1 tx pins name
   *			WM_IO_PB_06
   *
   * @return None
   */
  wm_uart1_tx_config (WM_IO_PB_06);

  /**
   * @brief  config the pins used for uart1 rx
   * @param  io_name: config uart1 rx pins name
   *			WM_IO_PB_07
   *			WM_IO_PB_16 only for 56pin
   *
   * @return None
   */
  wm_uart1_rx_config (WM_IO_PB_07);

  /**
   * @brief  config the pins used for uart1 rts
   * @param  io_name: config uart1 rts pins name
   *			WM_IO_PB_19
   *			WM_IO_PA_02 only for 56pin
   *
   * @return None
   */
  // wm_uart1_rts_config(WM_IO_PA_02);

  /**
   * @brief  config the pins used for uart1 cts
   * @param  io_name: config uart1 cts pins name
   *			WM_IO_PB_20
   *			WM_IO_PA_03 only for 56pin
   *
   * @return None
   */
  // wm_uart1_cts_config(WM_IO_PA_03);

  int i_Res = tls_ble_uart_init (BLE_UART_SERVER_MODE, uart_no, &opt);
  printf ("tls_ble_uart_init =%d\n", i_Res);

  return;
}
