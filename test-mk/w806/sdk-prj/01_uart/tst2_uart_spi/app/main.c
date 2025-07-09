/*****************************************************************************
 *
 * File Name : main.c
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wm_osal.h"

#include "wm_cpu.h"
#include "wm_watchdog.h"

#include <FreeRTOS.h>
#include <message_buffer.h>

#include "my_config.h"
#include "my_gpio.h"
#include "my_spi.h"
#include "my_uart.h"

static uint8_t ucStorageBuffer[DF_MESSAGEBUF_SIZE + 1];
/* The variable used to hold the message buffer structure. */
StaticMessageBuffer_t xMessageBufferStruct;
MessageBufferHandle_t xMessageBuffer = NULL;

void
UserMain (void)
{
  // init cpu clock
  tls_sys_clk_set (CPU_CLK_240M);

  // init watchDog
  tls_watchdog_init (30 * 1000 * 1000); // u32 usec microseconds, около 30 сек

  // init ringbuffer
  xMessageBuffer = xMessageBufferCreateStatic (
      DF_MESSAGEBUF_SIZE, ucStorageBuffer, &xMessageBufferStruct);

  // init gpio
  init_my_gpio (DF_CHECK_STATE_FILL);

  // init uart thread
  init_my_uart (xMessageBuffer);

  // init spi  thread
  init_my_spi (xMessageBuffer);
}
