
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_gpio_afsel.h"
#include "wm_hostspi.h"
#include "wm_osal.h"
#include "wm_type_def.h"

#include <FreeRTOS.h>
#include <message_buffer.h>

#include "my_config.h"
#include "my_spi.h"

#define USER_SPI_TASK_SIZE 1024
static OS_STK UserSpiTaskStk[USER_SPI_TASK_SIZE];
#define USER_SPI_TASK_PRIO 32

static u8 buffer[DF_TRANSMIT_BUF_SIZE + 1];

void
user_spi_task (void *sdata)
{

  MessageBufferHandle_t xMessageBuffer = (MessageBufferHandle_t)sdata;
  while (1)
    {
      size_t item_size;
      while (xMessageBuffer != NULL)
        {
          item_size = xMessageBufferReceive (
              xMessageBuffer, buffer, DF_TRANSMIT_BUF_SIZE, portMAX_DELAY);
          if (item_size > 0)
            {
              switch (tls_spi_write (buffer, item_size))
                {
                case TLS_SPI_STATUS_OK:
                  {
                    // printf("tls_spi_write TLS_SPI_STATUS_OK\n");
                  };
                  break;
                case TLS_SPI_STATUS_EINVAL:
                  {
                    printf ("tls_spi_write TLS_SPI_STATUS_EINVAL\n");
                    //        return WM_FAILED;
                  };
                  break;
                case TLS_SPI_STATUS_ENOMEM:
                  {
                    printf ("tls_spi_write TLS_SPI_STATUS_ENOMEM\n");
                    //        return WM_FAILED;
                  };
                  break;
                case TLS_SPI_STATUS_ESHUTDOWN:
                  {
                    printf ("tls_spi_write TLS_SPI_STATUS_ESHUTDOWN\n");
                    //        return WM_FAILED;
                  };
                  break;
                }
            }
          else
            {
              tls_os_time_delay (1);
            }
        }
    }
}

void
init_my_spi (void *xMessageBuffer)
{
  // init spi
  wm_spi_cs_config (DF_spi_cs_config);
  wm_spi_ck_config (DF_spi_ck_config);
  wm_spi_di_config (DF_spi_di_config);
  wm_spi_do_config (DF_spi_do_config);
  tls_spi_trans_type (SPI_DMA_TRANSFER);
  tls_spi_setup (DF_spi_mode, DF_spi_cs_active, DF_spi_clk);

  // init uart thread
  tls_os_task_create (NULL, "spi", user_spi_task, xMessageBuffer,
                      (void *)UserSpiTaskStk, /* task's stack start address */
                      USER_SPI_TASK_SIZE
                          * sizeof (u32), /* task's stack size, unit:byte */
                      USER_SPI_TASK_PRIO, 0);
}
