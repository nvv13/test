#if defined(__cplusplus)
extern "C"
{
#endif

#include "Arduino.h"

//#include "HTTPClient.h"
//#include "libemqtt.h"
#include "libwebsockets.h"
#include "list.h"
#include "lwip/inet.h"
#include "lwip/netif.h"
#include "private-libwebsockets.h"
#include "tls_common.h"
#include "wm_netif.h"
#include "wm_sockets.h"
#include <string.h>

#include "lwip/errno.h"
#include <stdio.h>

#if defined(__cplusplus)
}
#endif

#include "WiFiClientSecure.h"

WiFiClient::WiFiClient () { socket_num = -1; recv_ret=0; };

u32
WiFiClient::available (void) // есть доступные данные в принятом буфере
{
  if(socket_num==-1) return 0;
  if(recv_ret>0) return recv_ret;

  recv_ret = recv (socket_num, sock_rx, sizeof (sock_rx) - 1, 0);
  if (recv_ret < 0)
    {
      printf ("Receive failed, errno:%d\n", errno);
    }
  else
    {
      sock_rx[recv_ret] = 0;
      printf ("\nReceive %d bytes from \n", recv_ret); //,ip);
      //printf("%s\n",sock_rx);
    }
  tls_os_time_delay (2);

  return (recv_ret>0?recv_ret:0);
};

char
WiFiClient::read () // чтение по 1 char из потока (из буфера?), его и
                    // возвращает
{
  if(recv_ret<=0)return 0;
  char ch=sock_rx[recv_ret];
  recv_ret--;
  return ch;
};

int
WiFiClient::read (u8 *buf,
                  int len) //возвращает длинну прочтенных байтов в буфер
{

  //int ret = recv (socket_num, sock_rx, sizeof (sock_rx) - 1, 0);
  int ret = recv (socket_num, buf, len, 0);
  if (ret < 0)
    {
      printf ("Receive failed, errno:%d\n", errno);
    }
  else
    {
      //sock_rx[ret] = 0;
      printf ("\nReceive %d bytes from \n", ret); //,ip);
      //printf("%s\n",sock_rx);
    }
  tls_os_time_delay (2);

  return ret;
};

u16
WiFiClient::readBytes (char *buff, u16 len)
{
  return read ((u8 *)buff, len);
};

u16
WiFiClient::readBytes (u8 *buff, u16 len)
{
  return read (buff, len);
};

bool
WiFiClient::connected (void) //соединение есть
{
  return (socket_num != -1);
};

bool
WiFiClient::n_connect (char *host, u16 port)
{
  return n_connect (host, port, 3000);
};

void
WiFiClient::print (char *ch)
{
  u16 i_len_ret = sprintf (sock_tx, "%s", ch);
  int ret = send (socket_num, sock_tx, i_len_ret, 0);
  if (ret < 0)
    {
      printf ("Error occured during sending, errno:%d\n", errno);
    }
}; //пишем в поток сайта к которому подключились

void WiFiClient::stop (void)
{
      if (socket_num != -1)
        {
          printf ("shutting down socket and restaring...\n");
          shutdown (socket_num, 0);
          closesocket (socket_num);
          socket_num = -1;
          recv_ret=0;
        }

};

void WiFiClient::flush (void){}; // release memory

extern "C" unsigned long HTTPWrapperGetHostByName (char *name,
                                                   unsigned long *address);

bool
WiFiClient::n_connect (char *hostwoext, u16 port, u16 m_timeout_ms)
{

  timeout_ms = m_timeout_ms;

  unsigned long addr = 0;
  // Resolve the host name
  // unsigned long nRetCode =
  HTTPWrapperGetHostByName (hostwoext, &addr);

  struct sockaddr_in pin;

  memset (&pin, 0, sizeof (struct sockaddr));

  pin.sin_family = AF_INET;

  pin.sin_addr.s_addr = addr; // ipaddr_addr(ip);
  pin.sin_port = htons (port);

  recv_ret=0;
  socket_num = -1;

  if ((socket_num = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
      printf ("creat socket fail, errno:%d\n", errno);
      return -1;
    }

  if (connect (socket_num, (struct sockaddr *)&pin, sizeof (struct sockaddr))
      != 0)
    {
      printf ("connect fail errno:%d\n", errno);

      if (socket_num != -1)
        {
          printf ("shutting down socket and restaring...\n");
          shutdown (socket_num, 0);
          closesocket (socket_num);
          socket_num = -1;
          recv_ret=0;
        }
    }
  else
    {
      printf ("connect success\n");
    }

  tls_os_time_delay (HZ);

  return (socket_num != -1);
};
