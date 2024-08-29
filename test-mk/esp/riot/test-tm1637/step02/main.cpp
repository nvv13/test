/*
 * Copyright (C) 2019 Javier FILEIV <javier.fileiv@gmail.com>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file        main.c
 * @brief       Example using MQTT Paho package from RIOT
 *
 * @author      Javier FILEIV <javier.fileiv@gmail.com>
 *
 * @}
 */

extern "C" {
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "xtimer.h"
#include "shell.h"
#include "thread.h"
//#include "mutex.h"
 }
#include "TM1637Display.h"


static const shell_command_t shell_commands[] =
{
    { NULL,     NULL,                                 NULL        }
};

// Module connection pins (Digital Pins)

//#define ESP8266_BIT_DELAY   200
#define ESP8266_BIT_DELAY     100
#define ARDUINO_UNO_BIT_DELAY 100 

int main(void)
{
#ifdef MODULE_LWIP
    /* let LWIP initialize */
    xtimer_sleep(1);
#endif

//gpio_t CLK=GPIO_PIN(0,14); // D5 - lolin - nodemcu v3 - board - не подходят, платка не перезагружаеться!!
//gpio_t DIO=GPIO_PIN(0,15); // D8 - lolin - nodemcu v3 - board  

gpio_t CLK=GPIO_PIN(0,5); // D1 - lolin - nodemcu v3 - board
gpio_t DIO=GPIO_PIN(0,4); // D2 - lolin - nodemcu v3 - board  


  //TM1637Display* display=new TM1637Display(CLK, DIO);
  TM1637Display display(CLK, DIO, ESP8266_BIT_DELAY);
  display.setBrightness(0x01);
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };

  display.setSegments(data);
  xtimer_sleep(1);

while(true)
 {
  // Selectively set different digits
  data[0] = display.encodeDigit(0);
  data[1] = display.encodeDigit(1);
  data[2] = display.encodeDigit(2);
  data[3] = display.encodeDigit(3);
  display.setSegments(data);

  xtimer_sleep(1);
  data[0] = display.encodeDigit(4) + VIEW_POINT_DATA;
  data[1] = display.encodeDigit(5) + VIEW_POINT_DATA;
  data[2] = display.encodeDigit(6) + VIEW_POINT_DATA;
  data[3] = display.encodeDigit(7) + VIEW_POINT_DATA;
  display.setSegments(data);
  xtimer_sleep(1);
}
    printf("Running \n");


    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
