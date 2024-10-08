#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_type_def.h"

#include "wm_cpu.h"
#include "wm_rtc.h"
#include "wm_timer.h"
#include "wm_uart.h"
#include "wm_watchdog.h"

#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_osal.h"

#include "console_util.h"
#include "shell.h"

#define CONSOLE_BUF_SIZE 127
static u8 rx_buf[CONSOLE_BUF_SIZE + 1];
static volatile int rx_data_len;
static volatile u32 rptr;

static s16
proc_console_rx (u16 len, void *user_data)
{
  rx_data_len += len;
  return 0;
}

extern void dumpBuffer (char *name, char *buffer, int len);

void
console_task (void *sdata)
{
  shell_command_t *shell_commands = ((shell_command_t *)sdata);

  memset (rx_buf, 0, CONSOLE_BUF_SIZE + 1);
  rx_data_len = 0;
  rptr = 0;

  tls_uart_set_baud_rate (TLS_UART_0, 115200);
  tls_uart_rx_callback_register (TLS_UART_0, proc_console_rx, NULL);

  while (1)
    {
      tls_os_time_delay (HZ / 4);
      tls_watchdog_clr (); // пока есть управление, не перезапускаем!
      if (rx_data_len > 0)
        {
          if (rptr + rx_data_len > CONSOLE_BUF_SIZE)
            {
              printf ("rptr + rx_data_len > CONSOLE_BUF_SIZE\r\n");
              memset (rx_buf, 0, CONSOLE_BUF_SIZE + 1);
              rx_data_len = 0;
              rptr = 0;
            }
          else
            {
              int ret = tls_uart_read (TLS_UART_0, rx_buf + rptr, rx_data_len);
              if (ret > 0)
                {
                  // printf ("%s", (char *)(rx_buf + rptr));
                  // dumpBuffer ("\ruart0", (char *)rx_buf, 64);
                  /*
                  Клавиши - стрелки, кода:
                  uart0  Left: 1B, 5B, 44,
                  uart0    Up: 1B, 5B, 41,
                  uart0  Down: 1B, 5B, 42,
                  uart0 Right: 1B, 5B, 43,
                    backspace: 7F
                  */

                  char *pHeaderEnd = strstr ((char *)(rx_buf + rptr), "\r");
                  rx_data_len -= ret;
                  rptr += ret;
                  if (pHeaderEnd)
                    {
                      (*pHeaderEnd) = 0;
                      pHeaderEnd = (char *)rx_buf;
                      printf ("buf = \"%s\"\r\n", pHeaderEnd);

                      if (strstr (pHeaderEnd, "help") == pHeaderEnd)
                        {
                          printf ("usage:\r\n");
                          int ind_cmd = 0;
                          while (shell_commands[ind_cmd].name != NULL)
                            {
                              printf ("  \"%s\" : \"%s\"\r\n",
                                      shell_commands[ind_cmd].name,
                                      shell_commands[ind_cmd].desc);
                              ind_cmd++;
                            }
                        }
                      else
                        {
                          int ind_cmd = 0;
                          while (shell_commands[ind_cmd].name != NULL)
                            {
                              if (strstr (pHeaderEnd,
                                          shell_commands[ind_cmd].name)
                                  == pHeaderEnd)

                                {
                                  int argc = 0;
                                  char *argv[5];
                                  argv[argc] = pHeaderEnd;
                                  while (argc < 5)
                                    {
                                      argc++;
                                      char *pPos = strstr (pHeaderEnd, " ");
                                      if (pPos != NULL
                                          && pPos < ((
                                                 char *)(rx_buf
                                                         + CONSOLE_BUF_SIZE)))
                                        {
                                          *pPos = 0;
                                          pPos++;
                                          if (*pPos == 0)
                                            break;
                                          argv[argc] = pPos;
                                        }
                                      else
                                        break;
                                    }
                                  shell_commands[ind_cmd].handler (argc, argv);
                                  break;
                                }
                              ind_cmd++;
                            }
                        }

                      memset (rx_buf, 0, CONSOLE_BUF_SIZE + 1);
                      rx_data_len = 0;
                      rptr = 0;
                    }
                  else
                    {
                      pHeaderEnd = strstr ((char *)(rx_buf),
                                           "\x7f"); // backspace
                      if (pHeaderEnd)
                        {
                          (*pHeaderEnd) = 0;
                          rx_data_len = 0;
                          rptr--;
                          if (rptr > 0)
                            {
                              pHeaderEnd--;
                              (*pHeaderEnd) = 0;
                              rptr--;
                            }
                        }
                    }
                }
            }
        }
    }
}
