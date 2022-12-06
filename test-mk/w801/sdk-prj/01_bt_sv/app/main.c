/*****************************************************************************
 *
 * File Name : main.c
 *
 * Description: pin PB1, freq = 5,714309 MHz CPU_CLK_240M
 *
 * Date : 2022-05-20
 *****************************************************************************/

//#define DEMO_BT CFG_ON
//#define TLS_CONFIG_BR_EDR CFG_ON

#include "wm_type_def.h"
//#include "wm_cpu.h"
//#include "wm_gpio.h"
#include "wm_osal.h"
//#include "wm_regs.h"
#include <stdio.h>
#include <string.h>

#include "wm_mem.h"
#include "wm_uart.h"

#include "wm_bt_def.h"

#include "wm_bt_spp.h"

#include "wm_ble_uart_if.h"

//#if DEMO_BT
extern int demo_bt_enable (void *, ...);
extern int demo_ble_uart_server_on (uint8_t uart_no);
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

//#endif

/** WM BT SPP callback function */
// typedef void (*tls_bt_spp_callback_t)(tls_spp_event_t event, tls_spp_msg_t
// *p_data);

/*
void
spp_callback (tls_spp_event_t event, tls_spp_msg_t *p_data)
{
  switch ((int)event)
    {
    case WM_SPP_INIT_EVT:
      {
        printf ("WM_SPP_INIT_EVT\n");
        tls_spp_init_msg_t init_msg = p_data->init_msg;
      };
      break;
    case WM_SPP_DISCOVERY_COMP_EVT:
      {
        printf ("WM_SPP_DISCOVERY_COMP_EVT\n");
        tls_spp_disc_comp_msg_t disc_comp_msg = p_data->disc_comp_msg;
      };
      break;
    case WM_SPP_OPEN_EVT:
      {
        printf ("WM_SPP_OPEN_EVT\n");
        tls_spp_open_msg_t open_msg = p_data->open_msg;
      };
      break;
    case WM_SPP_CLOSE_EVT:
      {
        printf ("WM_SPP_CLOSE_EVT\n");
        tls_spp_close_msg_t close_msg = p_data->close_msg;
      };
      break;
    case WM_SPP_START_EVT:
      {
        printf ("WM_SPP_START_EVT\n");
        tls_spp_start_msg_t start_msg = p_data->start_msg;
      };
      break;
    case WM_SPP_CL_INIT_EVT:
      {
        printf ("WM_SPP_CL_INIT_EVT\n");
        tls_spp_cli_init_msg_t cli_init_msg = p_data->cli_init_msg;
      };
      break;
    case WM_SPP_DATA_IND_EVT:
      {
        printf ("WM_SPP_DATA_IND_EVT\n");
        tls_spp_data_ind_msg_t data_ind_msg = p_data->data_ind_msg;
      };
      break;
    case WM_SPP_CONG_EVT:
      {
        printf ("WM_SPP_CONG_EVT\n");
        tls_spp_cong_msg_t congest_msg = p_data->congest_msg;
      };
      break;
    case WM_SPP_WRITE_EVT:
      {
        printf ("WM_SPP_WRITE_EVT\n");
        tls_spp_write_msg_t write_msg = p_data->write_msg;
      };
      break;
    case WM_SPP_SRV_OPEN_EVT:
      {
        printf ("WM_SPP_SRV_OPEN_EVT\n");
        tls_spp_srv_open_msg_t srv_open_msg = p_data->srv_open_msg;
      };
      break;
    }
}
*/

#define LEN_BUFF 512
static uint8_t buffer[LEN_BUFF + 1];

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
  /*
  int tls_ble_uart_init(tls_ble_uart_mode_t mode, uint8_t uart_id,
  tls_uart_options_t *p_hci_if); int tls_ble_uart_deinit(tls_ble_uart_mode_t
  mode,uint8_t uart_id);

  uint32_t tls_ble_uart_buffer_size();
  uint32_t tls_ble_uart_buffer_available();
  uint32_t tls_ble_uart_buffer_read(uint8_t *ptr, uint32_t length);
  uint32_t tls_ble_uart_buffer_delete(uint32_t length);
  uint32_t tls_ble_uart_buffer_peek(uint8_t *ptr, uint32_t length);

  */

  uint8_t uart_no = 4; // 2;
  // int i_Res = demo_ble_uart_server_on (uart_no);

  int i_Res = tls_ble_uart_init (BLE_UART_SERVER_MODE, uart_no, NULL);
  printf ("demo_ble_uart_server_on =%d\n", i_Res);

  while (true)
    {
      memset (buffer, 0, LEN_BUFF + 1);
      //uint32_t u32_len = tls_ble_uart_buffer_read (buffer, LEN_BUFF);
      //uint32_t u32_len = tls_ble_uart_buffer_peek (buffer, LEN_BUFF);
    //  uint32_t u32_len = tls_ble_uart_buffer_size();
    //  printf ("tls_ble_uart_buffer_size=%d\n", u32_len);
//      if (u32_len > 0)
//        {
//          printf ("tls_ble_uart_buffer_read u32_len=%d,%s\n", u32_len, buffer);
//        }
    tls_os_time_delay (HZ);
    }

  /**spp realed api*/
  /**
   * @brief          Initializes the SPP interface
   *
   * @param[in]     callback      pointer on callback function
   *
   * @retval         @ref tls_bt_status_t
   *
   * @note           None
   */
  //  tls_bt_status_t t_stat = tls_bt_spp_init (spp_callback);
  //  printf ("tls_bt_spp_init tls_bt_status_t=%d\n", (int)t_stat);

  /**
   * @brief          Enable the bta jv interface
   *
   * @param[in]     None
   *
   * @retval         @ref tls_bt_status_t
   *
   * @note           None
   */
  //  t_stat = tls_bt_spp_enable ();
  //  printf ("tls_bt_spp_enable tls_bt_status_t=%d\n", (int)t_stat);

  /**
   * @brief       This function create a SPP server and starts listening
   for an
   *              SPP connection request from a remote Bluetooth device
   *
   * @param[in]   sec_mask:     Security Setting Mask .
   * @param[in]   role:         Server or client.
   * @param[in]   local_scn:    The specific channel you want to get.
   *                            If channel is 0, means get any channel.
   * @param[in]   name:         Server's name.
   *
   * @retval         @ref tls_bt_status_t

   */
  //  t_stat = tls_bt_spp_start_server (WM_SPP_SEC_NONE,
  //  WM_SPP_ROLE_SERVER, 0,
  //                                    "WM_12345");
  //  printf ("tls_bt_spp_start_server  tls_bt_status_t=%d\n",
  //  (int)t_stat);

  /**
   * @brief       This function is used to write data
   *
   * @param[in]   handle: The connection handle.
   * @param[in]   len:    The length of the data written.
   * @param[in]   p_data: The data written.
   *
   * @retval         @ref tls_bt_status_t

   */
  // tls_bt_status_t tls_bt_spp_write(uint32_t handle, uint8_t *p_data, int
  // length);

  /**
   * @brief          Disable the bta jv interface and cleanup internal
   * resource
   *
   * @param[in]     None
   *
   * @retval         @ref tls_bt_status_t
   *
   * @note           None
   */

  // t_stat = tls_bt_spp_disable();

  /**
   * @brief          Shuts down the SPP interface and does the cleanup
   *
   * @param       None
   *
   * @retval         @ref tls_bt_status_t
   *
   * @note           None
   */
  // t_stat = tls_bt_spp_deinit();

  return;
}
