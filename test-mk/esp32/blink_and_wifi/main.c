#include <stdio.h>
#include <string.h>

#include "shell.h"
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"

#include "net/ntp_packet.h"
#include "net/af.h"
#include <arpa/inet.h>
#include "net/sock/dns.h"

char stack[THREAD_STACKSIZE_MAIN];

#define IP_MAX_LEN_ADDRESS  (39)    /*IPv6 max length */
char addrstr[IP_MAX_LEN_ADDRESS];

extern struct netif *netif_default;

kernel_pid_t pid;
gpio_t pin13;

void *thread_handler(void *arg)
{
    (void) arg;
    puts("I'm in \"thread\" now");
    while(true)
     {
     gpio_write(pin13,1);
     xtimer_sleep(1);
     gpio_write(pin13,0);
     xtimer_sleep(1);
     //puts("blink ");

    struct netif *netif;
    if (((netif) = netif_default) != NULL)
    {
//          netif_default
//          uint8_t * addr = ((const ipv4_addr_t*)&((netif)->gw));//{0};
//          inet_ntop(AF_INET, addr , addrstr, sizeof(addrstr));
//          printf(" netif_default->gw  %s\n", addrstr);
    }

     }

    return NULL;
}


static void _usage1(char *cmd)
{
    printf("Usage: %s <server name>\n", cmd);
}


int _dns_query1(int argc, char **argv)
{
    if (argc < 2) {
        _usage1(argv[0]);
        return 1;
    }

    uint8_t addr[16] = {0};
    int res = sock_dns_query(argv[1], addr, AF_INET);
    if (res > 0) {
          inet_ntop(AF_INET, addr, addrstr, sizeof(addrstr));
          printf("%s resolves to %s\n", argv[1], addrstr);
    }
    else
    {
        puts("Error get ip");
        return 1;
    }

    return 0;
}

static const shell_command_t shell_commands[] =
{
    { "dnsquery", "dnsquery <name remote host>", _dns_query1 },
    { NULL,     NULL,                                 NULL        }
};



int main(void)
{

    xtimer_sleep(1);

    // configure DNS server .... 
    memset((void*)(&sock_dns_server),0,sizeof(sock_dns_server));
    sock_dns_server.addr.ipv4[0]=8;
    sock_dns_server.addr.ipv4[1]=8;
    sock_dns_server.addr.ipv4[2]=8;
    sock_dns_server.addr.ipv4[3]=8;
    sock_dns_server.port = SOCK_DNS_PORT;
    sock_dns_server.family = AF_INET;

    puts("This is blink");
    //xtimer_init();	
    pin13 = GPIO_PIN(0, 2);
    gpio_init(pin13,GPIO_OUT);


    /* ... */
    pid=thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_WOUT_YIELD, // THREAD_CREATE_STACKTEST,
                  thread_handler, NULL, "thread_handler");
                  
                  
    // thread_wakeup(pid);

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
