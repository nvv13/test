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
#include <arpa/inet.h>
#include "net/sock/dns.h"

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
//static const char time_google_com[] = "216.239.35.4";

static const char ntp_pool[] = "0.pool.ntp.org";
char addrstr[IP_MAX_LEN_ADDRESS];

time_t time_1=0; 
struct tm *tm_1;

time_t time_2=0; 
struct tm *tm_2;
//char str_t[32];
//size_t len;
//char* a_adc2[]={"cmd_set",NULL};

sock_udp_ep_t server_ntp = { .port = NTP_PORT, .family = AF_INET };

int _ntpdate1(int argc, char **argv)
{
    uint32_t timeout = _DEFAULT_TIMEOUT;

/*
    if (argc < 2) {
        _usage1(argv[0]);
        return 1;
    }
  */
    ipv4_addr_t *addr = (ipv4_addr_t *)&server_ntp.addr;


    //strncpy(_local_ip, argv[1], sizeof(_local_ip));
    //ipv4_addr_from_str(addr,argv[1]);
    //ipv4_addr_from_str(addr,time_google_com);

//if(/*(server_ntp.addr.ipv4[0]==0 && server_ntp.addr.ipv4[1]==0 && 
//    server_ntp.addr.ipv4[2]==0 && server_ntp.addr.ipv4[3]==0) ||*/ argc > 0)
//{
    //uint8_t addr_q[16] = {0};
    int res = sock_dns_query(ntp_pool, addr, AF_INET);
    if (res > 0) {
 /*
        server_ntp.addr.ipv4[0]=addr_q[0];
        server_ntp.addr.ipv4[1]=addr_q[1];
        server_ntp.addr.ipv4[2]=addr_q[2];
        server_ntp.addr.ipv4[3]=addr_q[3];
*/
        if (argc > 0)
          {
          inet_ntop(AF_INET, addr, addrstr, sizeof(addrstr));
          printf("%s resolves to %s\n", ntp_pool, addrstr);
          }
    }
    else
    {
        puts("Error get ip - in synchronization");
        //inet_ntop(AF_INET, sock_dns_server.addr.ipv4, addrstr, sizeof(addrstr));
        //printf("dns addr to %s\n", addrstr);
        return 1;
    }
//}

/*
    if (argc > 2) {
        timeout = atoi(argv[2]);
    }
*/
    if (sntp_sync(&server_ntp, timeout) < 0) {
        puts("Error in synchronization");
        return 1;
    }
//#if defined(MODULE_NEWLIB) || defined(MODULE_PICOLIBC)
    //struct tm *tm;
    time_1 = (time_t)(sntp_get_unix_usec() / US_PER_SEC);
    time_2=time_1;
 
   if (argc > 0)
     {
     tm_1 = gmtime(&time_1);
     printf("%04i-%02i-%02i %02i:%02i:%02i UTC (%i us)\n",
           tm_1->tm_year + 1900, tm_1->tm_mon + 1, tm_1->tm_mday,
           tm_1->tm_hour + HOUR_LOCAL_TIMEZONE, tm_1->tm_min, tm_1->tm_sec,
           (int)sntp_get_offset());
    }
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
   if(time_1<300)
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
   tm_2 = gmtime(&time_2);

   h_1=(tm_2->tm_hour + HOUR_LOCAL_TIMEZONE)/10;
   h_2=(tm_2->tm_hour + HOUR_LOCAL_TIMEZONE)%10;
   m_2=tm_2->tm_min/10;
   m_1=tm_2->tm_min%10;

   if(tm_2->tm_sec%2==0)
      PointData=VIEW_POINT_DATA;
     else
      PointData=0x00;


   data[0] = encodeDigit(h_1) + PointData;
   data[1] = encodeDigit(h_2) + PointData;
   data[2] = encodeDigit(m_2) + PointData;
   data[3] = encodeDigit(m_1) + PointData;
   setSegments(data,4,0);
   xtimer_sleep(1);
   
   time_2++;
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

    // configure DNS server .... 
    sock_dns_server.addr.ipv4[0]=8;
    sock_dns_server.addr.ipv4[1]=8;
    sock_dns_server.addr.ipv4[2]=8;
    sock_dns_server.addr.ipv4[3]=8;
    sock_dns_server.port = SOCK_DNS_PORT;
    sock_dns_server.family = AF_INET;

  /*
    server_ntp.port = NTP_PORT;
    server_ntp.family = AF_INET;
    server_ntp.addr.ipv4[0]=0;
    server_ntp.addr.ipv4[1]=0;
    server_ntp.addr.ipv4[2]=0;
    server_ntp.addr.ipv4[3]=0;
    */
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
