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

#define IP_MAX_LEN_ADDRESS  (39)    /*IPv6 max length */

#define _DEFAULT_TIMEOUT (500000LU)
//#define _DEFAULT_TIMEOUT (5000LU)

#define HOUR_LOCAL_TIMEZONE +3

extern int sntp_sync(sock_udp_ep_t *server, uint32_t timeout);
extern int64_t sntp_get_offset(void);

static void _usage1(char *cmd)
{
    printf("Usage: %s <server addr ipv4> [<timeout in us>]\n", cmd);
    printf("default: timeout = %lu\n", _DEFAULT_TIMEOUT);
}


//char _local_ip[IP_MAX_LEN_ADDRESS];
//static const char time_google_com[] = "216.239.35.4";

int _ntpdate1(int argc, char **argv)
{
    uint32_t timeout = _DEFAULT_TIMEOUT;

    if (argc < 2) {
        _usage1(argv[0]);
        return 1;
    }

    sock_udp_ep_t server = { .port = NTP_PORT, .family = AF_INET };
    ipv4_addr_t *addr = (ipv4_addr_t *)&server.addr;

    //strncpy(_local_ip, argv[1], sizeof(_local_ip));
    ipv4_addr_from_str(addr,argv[1]);
    //ipv4_addr_from_str(addr,time_google_com);



    if (argc > 2) {
        timeout = atoi(argv[2]);
    }
    if (sntp_sync(&server, timeout) < 0) {
        puts("Error in synchronization");
        return 1;
    }
#if defined(MODULE_NEWLIB) || defined(MODULE_PICOLIBC)
    struct tm *tm;
    time_t time = (time_t)(sntp_get_unix_usec() / US_PER_SEC);

    tm = gmtime(&time);
    printf("%04i-%02i-%02i %02i:%02i:%02i UTC (%i us)\n",
           tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
           tm->tm_hour + HOUR_LOCAL_TIMEZONE, tm->tm_min, tm->tm_sec,
           (int)sntp_get_offset());
#else
    uint64_t time = sntp_get_unix_usec();
    printf("%" PRIu32 ".%" PRIu32 " (%i us)\n",
           (uint32_t)(time / US_PER_SEC),
           (uint32_t)(time / US_PER_SEC),
           (int)sntp_get_offset());
#endif
    return 0;
}


static const shell_command_t shell_commands[] =
{
    { "ntpdate", "synchronizes with a remote time server", _ntpdate1 },
    { NULL,     NULL,                                 NULL        }
};

char stack[THREAD_STACKSIZE_MAIN];

int i_cout=0;

void *thread_handler(void *arg)
{
    /* ... */
    (void)arg;

    while(true)
     {
     xtimer_sleep(5);
     printf("thread count %d \n",i_cout++);
     }
}

int main(void)
{
#ifdef MODULE_LWIP
    /* let LWIP initialize */
    xtimer_sleep(1);
#endif

    thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  thread_handler, NULL,
                  "thread");
   

    printf("Running \n");


    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
