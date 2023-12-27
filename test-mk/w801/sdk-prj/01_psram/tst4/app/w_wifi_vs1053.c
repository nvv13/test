#include "mod1/VS1053.h"
#include "w_wifi.h"
#include <string.h>


u8 volatile u8_wifi_state = 0;

static void
con_net_status_changed_event (u8 status)
{
  switch (status)
    {
    case NETIF_WIFI_JOIN_SUCCESS:
      printf ("NETIF_WIFI_JOIN_SUCCESS\n");
      u8_wifi_state = 1;
      break;
    case NETIF_WIFI_JOIN_FAILED:
      printf ("NETIF_WIFI_JOIN_FAILED\n");
      u8_wifi_state = 0;
      break;
    case NETIF_WIFI_DISCONNECTED:
      {
        printf ("NETIF_WIFI_DISCONNECTED\n");
        if (my_sost == VS1053_PLAY)
          my_sost = VS1053_PLAY_BUF;
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

// acitve connect to specified AP, use command as: t-connet("ssid","pwd");
int
wifi_connect (const char *ssid, const char *pwd)
{
  struct tls_param_ip *ip_param = NULL;
  u8 wireless_protocol = 0;

  if (!ssid)
    {
      return WM_FAILED;
    }

  printf ("\nssid:%s\n", ssid);
  printf ("password=***\n"); //, pwd);
  tls_wifi_disconnect ();

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
