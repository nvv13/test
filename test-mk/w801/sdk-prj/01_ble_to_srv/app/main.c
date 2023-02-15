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

#include "wm_bt_def.h"
#include "../src/app/bleapp/wm_ble_server_api_demo.h"

extern int demo_bt_enable (void);

/**uart output function pointer, ble server send the received data to uart */
void my_output_ptr(tls_uart_msg_out_t type,uint8_t *payload, int length)
{
switch((int)type)
 {
  case  UART_OUTPUT_DATA            : break;
  case  UART_OUTPUT_CMD_ADVERTISING : break;
  case  UART_OUTPUT_CMD_CONNECTED   : break;
  case  UART_OUTPUT_CMD_DISCONNECTED: break;
 }
printf("my_output_ptr type:%d length:%d\n",(int)type ,length);
if(length>0)printf("payload:%s\n",payload);

                //отправим обратно 
int i_Res = tls_ble_server_demo_api_send_msg_notify(payload, length);
printf ("tls_ble_server_demo_api_send_msg_notify =%d\n", i_Res);

};

/**uart sent function pointer, after ble server sending the uart data, it will be called */
void my_in_and_sent_ptr(tls_ble_uart_mode_t mode, int status)
{
switch((int)mode)
 {
  case    BLE_UART_SERVER_MODE : break;
  case    BLE_UART_CLIENT_MODE : break;
  case    BLE_UART_UNKNOWN_MODE: break;
 }
printf("my_in_and_sent_ptr mode:%d status:%d\n",(int)mode ,status);
};

void
UserMain (void)
{

  printf ("user task\n");

  demo_bt_enable ();
  printf ("demo_bt_enable... delay 5 sec\n");
  tls_os_time_delay (HZ * 5);


  int i_Res = tls_ble_server_demo_api_init(my_output_ptr, my_in_and_sent_ptr);
  printf ("tls_ble_uart_init =%d\n", i_Res);

  //tls_ble_server_demo_api_deinit();

  return;
}
