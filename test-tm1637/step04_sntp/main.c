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

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "xtimer.h"
#include "shell.h"
#include "thread.h"
//#include "mutex.h"

#include <time.h>
#include "net/sntp.h"
#include "net/ntp_packet.h"
#include "net/af.h"
//#include "net/ipv6/addr.h"
#include "timex.h"

#include "TM1637Display.h"


#define IP_MAX_LEN_ADDRESS  (39)    /*IPv6 max length */

#define _DEFAULT_TIMEOUT (500000LU)
//#define _DEFAULT_TIMEOUT (5000LU)

#define HOUR_LOCAL_TIMEZONE +3

extern int sntp_sync(sock_udp_ep_t *server, uint32_t timeout);
extern int64_t sntp_get_offset(void);

/*
static void _usage1(char *cmd)
{
    printf("Usage: %s <server addr ipv4> [<timeout in us>]\n", cmd);
    printf("default: timeout = %lu\n", _DEFAULT_TIMEOUT);
}
*/

//char _local_ip[IP_MAX_LEN_ADDRESS];
static const char time_google_com[] = "216.239.35.4";

time_t time_1=0; 
struct tm *tm;
//char str_t[32];
//size_t len;
//char* a_adc2[]={"cmd_set",NULL};

int _ntpdate1(int argc, char **argv)
{
    uint32_t timeout = _DEFAULT_TIMEOUT;

/*
    if (argc < 2) {
        _usage1(argv[0]);
        return 1;
    }
  */

    sock_udp_ep_t server = { .port = NTP_PORT, .family = AF_INET };
    ipv4_addr_t *addr = (ipv4_addr_t *)&server.addr;

    //strncpy(_local_ip, argv[1], sizeof(_local_ip));
    //ipv4_addr_from_str(addr,argv[1]);
    ipv4_addr_from_str(addr,time_google_com);


/*
    if (argc > 2) {
        timeout = atoi(argv[2]);
    }
*/
    if (sntp_sync(&server, timeout) < 0) {
        puts("Error in synchronization");
        return 1;
    }
//#if defined(MODULE_NEWLIB) || defined(MODULE_PICOLIBC)
    //struct tm *tm;
    time_1 = (time_t)(sntp_get_unix_usec() / US_PER_SEC);

    tm = gmtime(&time_1);
 
   if (argc > 0)
     printf("%04i-%02i-%02i %02i:%02i:%02i UTC (%i us)\n",
           tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
           tm->tm_hour + HOUR_LOCAL_TIMEZONE, tm->tm_min, tm->tm_sec,
           (int)sntp_get_offset());
//#else
//    uint64_t time = sntp_get_unix_usec();
//    printf("%" PRIu32 ".%" PRIu32 " (%i us)\n",
//           (uint32_t)(time / US_PER_SEC),
//           (uint32_t)(time / US_PER_SEC),
//           (int)sntp_get_offset());
//#endif
    return 0;
}



char stack_ntp_update[THREAD_STACKSIZE_MAIN];

void *thread_handler_ntp_update(void *arg)
{
    /* ... */
    (void)arg;
   xtimer_sleep(5);
  _ntpdate1(0,NULL);

  while(true)
   {
   if(tm->tm_hour==0 && tm->tm_min<3 )
    {
    _ntpdate1(0,NULL);
    xtimer_sleep(30);
    }
    else
    {
    _ntpdate1(0,NULL);
    xtimer_sleep(600);
    }
  }

}


char stack_clock[THREAD_STACKSIZE_MAIN];

uint8_t PointData = 0x00;
unsigned char h_1=0;
unsigned char h_2=0;
unsigned char m_1=0;
unsigned char m_2=0;

void *thread_handler_clock(void *arg)
{
    /* ... */
    (void)arg;

  setBrightness(0x01,1);
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  setSegments(data,4,0);

  while(true)
   {
   // Selectively set different digits

     tm = gmtime(&time_1);

     h_1=(tm->tm_hour + HOUR_LOCAL_TIMEZONE)/10;
     h_2=(tm->tm_hour + HOUR_LOCAL_TIMEZONE)%10;

     m_2=tm->tm_min/10;
     m_1=tm->tm_min%10;

     if(tm->tm_sec%2==0)
       PointData=VIEW_POINT_DATA;
      else
       PointData=0x00;


  data[0] = encodeDigit(h_1) + PointData;
  data[1] = encodeDigit(h_2) + PointData;
  data[2] = encodeDigit(m_2) + PointData;
  data[3] = encodeDigit(m_1) + PointData;
  setSegments(data,4,0);
  xtimer_sleep(1);

  time_1++;
  }

}






static const shell_command_t shell_commands[] =
{
    { "ntpdate", "synchronizes with a remote time server", _ntpdate1 },
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
  TM1637Display(CLK, DIO, ESP8266_BIT_DELAY);

  thread_create(stack_clock, sizeof(stack_clock),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  thread_handler_clock, NULL,
                  "clock_update");

   thread_create(stack_ntp_update, sizeof(stack_ntp_update),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  thread_handler_ntp_update, NULL,
                  "ntp_update");


    printf("Running \n");


    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
