#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_uart.h"

#include "wm_gpio_afsel.h"
#include "wm_osal.h"
#include "wm_watchdog.h"

#include <FreeRTOS.h>
#include <message_buffer.h>

#include "my_gpio.h"

#include "my_config.h"
#include "my_uart.h"

#define USER_UART_TASK_SIZE 1024
static OS_STK UserUartTaskStk[USER_UART_TASK_SIZE];
#define USER_UART_TASK_PRIO 32

static u8 rx_buf[DF_TRANSMIT_BUF_SIZE + 1];
volatile int rx_data_len;
volatile u32 rptr;

s16
proc_console_rx (u16 len, void *user_data)
{
  rx_data_len += len;
  return 0;
}

void
user_uart_task (void *sdata)
{
  MessageBufferHandle_t xMessageBuffer = (MessageBufferHandle_t)sdata;
  while (1)
    {
      tls_watchdog_clr (); // пока есть управление, не перезапускаем!
      if (rx_data_len > 0)
        {
          if ((rptr + rx_data_len) > DF_TRANSMIT_BUF_SIZE)
            { // buffer overload
              rx_data_len = 0;
              rptr = 0;
            }
          else
            {
              int ret
                  = tls_uart_read (DF_UART_SEL, rx_buf + rptr, rx_data_len);
              if (ret > 0)
                {
                  rx_data_len -= ret;
                  rptr += ret;
                }
            }
        }
      else
        {
          if (!gpioStateFill ())
            { // gpioStateFill () - пин для проверки состояния ввода данных
              //    (0-данные идут, 1-надо отправлять что уже принял)
              if (rptr > 0)
                { // rptr > 0 - данные есть
                  size_t freeSize = 0;
                  while (1)
                    {
                      freeSize
                          = xMessageBufferSpacesAvailable (xMessageBuffer);
                      if (freeSize < rptr)
                        tls_os_time_delay (1);
                      else
                        break;
                    }
                  xMessageBufferSend (xMessageBuffer, rx_buf, rptr,
                                      portMAX_DELAY);
                }
              rx_data_len = 0;
              rptr = 0;
            }
          tls_os_time_delay (1);
        }
    }
}

void
init_my_uart (void *xMessageBuffer)
{
  // prepare variable
  memset (rx_buf, 0, DF_TRANSMIT_BUF_SIZE + 1);
  rx_data_len = 0;
  rptr = 0;
  tls_uart_options_t opt;

  // init uart port
  if (DF_UART_SEL != TLS_UART_0)
    {
      wm_uart1_rx_config (DF_UART_rx_config);
      wm_uart1_tx_config (DF_UART_tx_config);

      opt.baudrate = DF_UART_BAUDRATE;
      opt.paritytype = DF_UART_paritytype;
      opt.stopbits = DF_UART_stopbits;
      opt.charlength = DF_UART_charlength;
      opt.flow_ctrl = DF_UART_flow_ctrl;

      if (WM_SUCCESS != tls_uart_port_init (DF_UART_SEL, &opt, 0))
        {
          printf ("uart init error\n");
        }
    }
  else
    {
      tls_uart_set_baud_rate (DF_UART_SEL, DF_UART_BAUDRATE);
    }
  tls_uart_rx_callback_register (DF_UART_SEL, proc_console_rx, NULL);

  // init uart thread
  tls_os_task_create (NULL, "uart", user_uart_task, xMessageBuffer,
                      (void *)UserUartTaskStk, /* task's stack start address */
                      USER_UART_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_UART_TASK_PRIO, 0);
}
