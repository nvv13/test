#include <string.h>
//#include "wm_wifi_oneshot.h"
#include "wm_osal.h"

//#include "decode_cmd.h"
#include "w_wifi.h"

#define DEMO_SOCK_BUF_SIZE 1024
static char sock_rx[DEMO_SOCK_BUF_SIZE] = { 0 };

static int new_fd = -1;
static int server_fd = -1;
static int i_start_recive = -1;

extern u8 u8_wifi_state;

static void
con_net_status_changed_event (u8 status)
{
  switch (status)
    {
    case NETIF_WIFI_JOIN_SUCCESS:
      printf ("NETIF_WIFI_JOIN_SUCCESS\n");
      break;
    case NETIF_WIFI_JOIN_FAILED:
      printf ("NETIF_WIFI_JOIN_FAILED\n");
      u8_wifi_state = 0;
      break;
    case NETIF_WIFI_DISCONNECTED:
      {
        printf ("NETIF_WIFI_DISCONNECTED\n");
        if (new_fd != -1)
          closesocket (new_fd);
        new_fd = -1;
        if (server_fd != -1)
          closesocket (server_fd);
        server_fd = -1;
      u8_wifi_state = 0;
      };
      break;
    case NETIF_IP_NET_UP:
      {
        struct tls_ethif *tmpethif = tls_netif_get_ethif ();
        print_ipaddr (&tmpethif->ip_addr);
#if TLS_CONFIG_IPV6
        print_ipaddr (&tmpethif->ip6_addr[0]);
        print_ipaddr (&tmpethif->ip6_addr[1]);
        print_ipaddr (&tmpethif->ip6_addr[2]);
#endif
        //	printf("net up ==> ip =
        //%d.%d.%d.%d\n",ip4_addr1(&tmpethif->ip_addr.addr),ip4_addr2(&tmpethif->ip_addr.addr),
        //							 ip4_addr3(&tmpethif->ip_addr.addr),ip4_addr4(&tmpethif->ip_addr.addr));
      }
      break;
    default:
      // printf("UNKONWN STATE:%d\n", status);
      break;
    }
}

int
create_socket_server (const int port)
{

  struct sockaddr_in client_addr; // connector's address information
  socklen_t sin_size;
  int ret;

  while (1)
    {
      struct tls_ethif *ethif;
      while (1)
        {
          ethif = tls_netif_get_ethif ();
          if (ethif->status) //ждем wifi
            break;
          printf ("wait wifi\n");
          tls_os_time_delay (500);
        }
      if (server_fd == -1)
        {
          struct sockaddr_in server_addr; // server address information
          if ((server_fd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
            {
              printf ("create socket fail,errno :%d\n", errno);
              break;
            }

          server_addr.sin_family = AF_INET;
          server_addr.sin_port = htons (port);
          server_addr.sin_addr.s_addr = ((u32)0x00000000UL);
          memset (server_addr.sin_zero, '\0', sizeof (server_addr.sin_zero));
          if (bind (server_fd, (struct sockaddr *)&server_addr,
                    sizeof (server_addr))
              != 0)
            {
              printf ("bind fail, errno:%d\n", errno);
              break;
            }
        }

      if (listen (server_fd, 1) != 0)
        {
          printf ("listen fail,errno:%d\n", errno);
          break;
        }

      printf ("listen port %d\n", port);
      sin_size = sizeof (client_addr);

      struct timeval tmv;
      // Initialize the timeout data structure
      tmv.tv_sec = 3;
      tmv.tv_usec = 0;
      int i_ret = setsockopt (server_fd, SOL_SOCKET, SO_RCVTIMEO,
                              (struct timeval *)&tmv, sizeof (struct timeval));
      printf ("server_fd setsockopt i_ret=%d\n", i_ret);
      new_fd = -1;
      while (new_fd == -1
             && server_fd != -1) // это сделано, чтоб в этом месте программа не
                                 // застряла, при переподключении к wifi
        {
          // printf("start accept\n");
          i_start_recive = server_fd;
          new_fd
              = accept (server_fd, (struct sockaddr *)&client_addr, &sin_size);
          // printf("accept newfd = %d %d\n",new_fd,i_ret++);
          i_start_recive = -1;
        }

      if (new_fd < 0)
        {
          printf ("accept fail,errno:%d\n", errno);
          break;
        }

      while (1)
        {
          memset (sock_rx, 0, DEMO_SOCK_BUF_SIZE);

          tmv.tv_sec = 3;
          tmv.tv_usec = 0;
          i_ret = setsockopt (new_fd, SOL_SOCKET, SO_RCVTIMEO,
                              (struct timeval *)&tmv, sizeof (struct timeval));
          printf ("new_fd setsockopt SO_RCVTIMEO i_ret=%d\n", i_ret);
          ret = -1;
          u8 u8_count_max_to_recive_timeout
              = 100; // 3 сек * 100 = 5 мин ждем, и отсоединяем, если нет
                     // комманды....
          while (ret == -1 && new_fd != -1
                 && u8_count_max_to_recive_timeout
                        > 0) // это сделано, чтоб в этом месте программа не
                             // застряла, при переподключении к wifi
            {
              // printf("start recv\n");
              i_start_recive = new_fd;
              ret = recv (new_fd, sock_rx, sizeof (sock_rx) - 1, 0);
              i_start_recive = -1;
              u8_count_max_to_recive_timeout--;
              // printf("recv ret=%d %d\n", ret,i_ret++);
            }

          if (ret == 0)
            {
              printf ("connection disconnect\n");
              break;
            }
          else if (ret < 0)
            {
              printf ("receive fail errno:%d\n", errno);
              break;
            }
          else
            {
              sock_rx[ret] = 0;
              printf ("\nReceive %d bytes from %s\n", ret,
                      inet_ntoa (client_addr.sin_addr.s_addr));
              printf ("%s\n", sock_rx);
              /*
                            int i_len_ret
                                = fast_decode_cmd (sock_rx, ret); // atoi((char
                 *)buf + 11); if (i_len_ret > 0)
                              {
                                ret = send (new_fd, sock_rx, i_len_ret, 0);
                                if (ret < 0)
                                  {
                                    printf ("Error occured during
                 sending,errno:%d\n", errno); break;
                                  }
                              }
              */
            }
        }
      if (new_fd != -1)
        {
          printf ("shutting down socket and restaring...\n");
          shutdown (new_fd, 0);
          closesocket (new_fd);
          new_fd = -1;
        }
    }

  return 0;
}

/*
int demo_oneshot(void)
{
    printf("waiting for oneshot \n");
    tls_netif_add_status_event(con_net_status_changed_event);
    tls_wifi_set_oneshot_flag(1);
    return 0;
}

int demo_socket_config(void)
{
    printf("AP Mode socket config demo \n");
    tls_netif_add_status_event(con_net_status_changed_event);
    tls_wifi_set_oneshot_flag(2);
    return 0;
}

int demo_webserver_config(void)
{
    printf("AP Mode web server config mode \n");
    tls_netif_add_status_event(con_net_status_changed_event);
    tls_wifi_set_oneshot_flag(3);
    return 0;
}
*/

// acitve connect to specified AP, use command as: t-connet("ssid","pwd");
int
demo_connect_net (const char *ssid, const char *pwd)
{
  struct tls_param_ip *ip_param = NULL;
  u8 wireless_protocol = 0;

  if (!ssid)
    {
      return WM_FAILED;
    }

  if (new_fd != -1)
    {
      printf ("shutting down socket ...\n");
      shutdown (new_fd, 0);
      closesocket (new_fd);
      new_fd = -1;
    }
  if (server_fd != -1)
    {
      printf ("close socket server...\n");
      shutdown (server_fd, 0);
      closesocket (server_fd);
      server_fd = -1;
    }

  printf ("\nssid:%s\n", ssid);
  printf ("password=%s\n", pwd);
  tls_wifi_disconnect ();

  while (i_start_recive != -1)
    {
      printf ("wait i_start_recive... \n");
      tls_os_time_delay (1000);
    }

  tls_param_get (TLS_PARAM_ID_WPROTOCOL, (void *)&wireless_protocol, TRUE);
  if (TLS_PARAM_IEEE80211_INFRA != wireless_protocol)
    {
      tls_wifi_softap_destroy ();
      wireless_protocol = TLS_PARAM_IEEE80211_INFRA;
      tls_param_set (TLS_PARAM_ID_WPROTOCOL, (void *)&wireless_protocol,
                     FALSE);
    }

  tls_wifi_set_oneshot_flag (0);

  ip_param = tls_mem_alloc (sizeof (struct tls_param_ip));
  if (ip_param)
    {
      tls_param_get (TLS_PARAM_ID_IP, ip_param, FALSE);
      ip_param->dhcp_enable = TRUE;
      tls_param_set (TLS_PARAM_ID_IP, ip_param, FALSE);
      tls_mem_free (ip_param);
    }

  tls_netif_add_status_event (con_net_status_changed_event);
  tls_wifi_connect ((u8 *)ssid, strlen (ssid), (u8 *)pwd, strlen (pwd));
  printf ("\nplease wait connect net......\n");

  return WM_SUCCESS;
}
