/*****************************************************************************
*
* File Name : wm_scan_demo.c
*
* Description: wifi scan demo function
*
* Copyright (c) 2018 Winner Micro Electronic Design Co., Ltd.
* All rights reserved.
*
* Author : muqing
*
* Date : 2018-12-03
*****************************************************************************/


#include <string.h>
#include "wm_include.h"
#include "tls_wireless.h"

#if DEMO_SCAN

static  char *scan_privacy_string(u8 privacy)
{
    char *sec;

    switch (privacy)
    {
    case WM_WIFI_AUTH_MODE_OPEN:
        sec = "NONE";
        break;
    case WM_WIFI_AUTH_MODE_WEP_AUTO:
        sec = "WEP/AUTO";
        break;
    case WM_WIFI_AUTH_MODE_WPA_PSK_TKIP:
        sec = "WPA_PSK/TKIP";
        break;
    case WM_WIFI_AUTH_MODE_WPA_PSK_CCMP:
        sec = "WPA_PSK/CCMP";
        break;
    case WM_WIFI_AUTH_MODE_WPA_PSK_AUTO:
        sec = "WPA_PSK/AUTO";
        break;
    case WM_WIFI_AUTH_MODE_WPA2_PSK_TKIP:
        sec = "WPA2_PSK/TKIP";
        break;
    case WM_WIFI_AUTH_MODE_WPA2_PSK_CCMP:
        sec = "WPA2_PSK/CCMP";
        break;
    case WM_WIFI_AUTH_MODE_WPA2_PSK_AUTO:
        sec = "WPA2_PSK/AUTO";
        break;
    case WM_WIFI_AUTH_MODE_WPA_WPA2_PSK_TKIP:
        sec = "WPA_PSK/WPA2_PSK/TKIP";
        break;
    case WM_WIFI_AUTH_MODE_WPA_WPA2_PSK_CCMP:
        sec = "WPA_PSK/WPA2_PSK/CCMP";
        break;
    case WM_WIFI_AUTH_MODE_WPA_WPA2_PSK_AUTO:
        sec = "WPA_PSK/WPA2_PSK/AUTO";
        break;

    default:
        sec = "Unknown";
        break;
    }
    return sec;
}

static  char *scan_mode_string(u8 mode)
{
    char *ap_mode;

    switch (mode)
    {
    case 1:
        ap_mode = "IBSS";
        break;
    case 2:
        ap_mode = "ESS";
        break;

    default:
        ap_mode = "ESS";
        break;
    }
    return ap_mode;
}


static void wifi_scan_handler(void)
{
    char *buf = NULL;
    char *buf1 = NULL;
    u32 buflen;
    int i, j;
    int err;
    u8 ssid[33];
    struct tls_scan_bss_t *wsr;
    struct tls_bss_info_t *bss_info;

    buflen = 2000;
    buf = tls_mem_alloc(buflen);
    if (!buf)
    {
        goto end;
    }

    buf1 = tls_mem_alloc(300);
    if(!buf1)
    {
        goto end;
    }
    memset(buf1, 0, 300);

    err = tls_wifi_get_scan_rslt((u8 *)buf, buflen);
    if (err)
    {
        goto end;
    }

    wsr = (struct tls_scan_bss_t *)buf;
    bss_info = (struct tls_bss_info_t *)(buf + 8);

    printf("\n");

    for(i = 0; i < wsr->count; i++)
    {
        j = sprintf(buf1, "bssid:%02X%02X%02X%02X%02X%02X, ", bss_info->bssid[0], bss_info->bssid[1],
                    bss_info->bssid[2], bss_info->bssid[3], bss_info->bssid[4], bss_info->bssid[5]);
        j += sprintf(buf1 + j, "ch:%d, ", bss_info->channel);
        j += sprintf(buf1 + j, "rssi:%d, ", (signed char)bss_info->rssi);
        j += sprintf(buf1 + j, "wps:%d, ", bss_info->wps_support);
        j += sprintf(buf1 + j, "max_rate:%dMbps, ", bss_info->max_data_rate);
        j += sprintf(buf1 + j, "%s, ", scan_mode_string(bss_info->mode));
        j += sprintf(buf1 + j, "%s, ", scan_privacy_string(bss_info->privacy));
        memcpy(ssid, bss_info->ssid, bss_info->ssid_len);
        ssid[bss_info->ssid_len] = '\0';
        j += sprintf(buf1 + j, "%s", ssid);

        printf("%s\n", buf1);

        bss_info ++;
    }

end:
    if(buf)
    {
        tls_mem_free(buf);
    }
    if(buf1)
    {
        tls_mem_free(buf1);
    }
}

static void wifi_scan_format2_handler(void)
{
    char *buf = NULL;
    char *buf1 = NULL;
    u32 buflen;
    int i, j;
    int err;
    u8 ssid[33];
    struct tls_scan_bss_format2_t *wsr;
    struct tls_bss_info_format2_t *bss_info;

    buflen = 2000;
    buf = tls_mem_alloc(buflen);
    if (!buf)
    {
        goto end;
    }

    buf1 = tls_mem_alloc(300);
    if(!buf1)
    {
        goto end;
    }
    memset(buf1, 0, 300);

    err = tls_wifi_get_scan_rslt_format2((u8 *)buf, buflen);
    if (err)
    {
        goto end;
    }

    wsr = (struct tls_scan_bss_format2_t *)buf;
    bss_info = (struct tls_bss_info_format2_t *)(buf + 8);

    printf("\n");

    for(i = 0; i < wsr->count; i++)
    {
    	j = sprintf(buf1, "(%d,", bss_info->ecn);
        memcpy(ssid, bss_info->ssid, bss_info->ssid_len);
        ssid[bss_info->ssid_len] = '\0';

        j += sprintf(buf1 + j, "%s", ssid);

        j += sprintf(buf1 + j, ",%d, ", (signed char)bss_info->rssi);

        j += sprintf(buf1 + j, "\"%02x:%02x:%02x:%02x:%02x:%02x\", ", bss_info->bssid[0], bss_info->bssid[1],
                    bss_info->bssid[2], bss_info->bssid[3], bss_info->bssid[4], bss_info->bssid[5]);

        j += sprintf(buf1 + j, "%d, ", bss_info->channel);

        j += sprintf(buf1 + j, "%d, ", bss_info->group_cipher);

        j += sprintf(buf1 + j, "%d, ", bss_info->pairwise_cipher);		

        j += sprintf(buf1 + j, "%d, ", bss_info->bgn);

        j += sprintf(buf1 + j, "%d,", bss_info->wps_support);
		
        j += sprintf(buf1 + j, "%d,", bss_info->freq_offset);
		
        j += sprintf(buf1 + j, "%d)", bss_info->freqcal_val);		


        printf("%s\n", buf1);

        bss_info ++;
    }

end:
    if(buf)
    {
        tls_mem_free(buf);
    }
    if(buf1)
    {
        tls_mem_free(buf1);
    }
}


struct specified_scan_info
{
	u8 specified_ssid[33];
	u16 specified_ssid_len;
	u8 specified_bssid[18];
	u8 specified_chan;
	u8 specified_mask;
};

struct specified_scan_info specifiedinfo;

static void wifi_scan_format2_filter_handler(void)
{
    char *buf = NULL;
    char *buf1 = NULL;
    u32 buflen;
    int i, j;
    int err;
    u8 ssid[33];
	u8 bssid[18];
    struct tls_scan_bss_format2_t *wsr;
    struct tls_bss_info_format2_t *bss_info;
	u8 bitmask = 0;

    buflen = 2000;
    buf = tls_mem_alloc(buflen);
    if (!buf)
    {
        goto end;
    }

    buf1 = tls_mem_alloc(300);
    if(!buf1)
    {
        goto end;
    }
    memset(buf1, 0, 300);

    err = tls_wifi_get_scan_rslt_format2((u8 *)buf, buflen);
    if (err)
    {
        goto end;
    }

    wsr = (struct tls_scan_bss_format2_t *)buf;
    bss_info = (struct tls_bss_info_format2_t *)(buf + 8);

    printf("\n");
	bitmask = specifiedinfo.specified_mask;
    for(i = 0; i < wsr->count; i++)
    {
    	j = sprintf(buf1, "(%d,", bss_info->ecn);
		if (bitmask&0x1) 
		{
			if((specifiedinfo.specified_ssid_len != bss_info->ssid_len) 
				|| memcmp(bss_info->ssid,specifiedinfo.specified_ssid,bss_info->ssid_len))
			{
				bss_info++;
				continue;
			}
		}
   		memcpy(ssid, bss_info->ssid, bss_info->ssid_len);
        ssid[bss_info->ssid_len] = '\0';

        j += sprintf(buf1 + j, "%s", ssid);

        j += sprintf(buf1 + j, ",%d,", (signed char)bss_info->rssi);

		if (bitmask&0x2)
		{
			sprintf((char *)bssid, "%02x:%02x:%02x:%02x:%02x:%02x", bss_info->bssid[0], bss_info->bssid[1],
                    bss_info->bssid[2], bss_info->bssid[3], bss_info->bssid[4], bss_info->bssid[5]);
			if (strcmp((const char *)bssid, (const char *)specifiedinfo.specified_bssid))
			{
				bss_info++;
				continue;	
			}
		}
        j += sprintf(buf1 + j, "\"%02x:%02x:%02x:%02x:%02x:%02x\",", bss_info->bssid[0], bss_info->bssid[1],
                    bss_info->bssid[2], bss_info->bssid[3], bss_info->bssid[4], bss_info->bssid[5]);

		if (bitmask&0x4)
		{
			if (bss_info->channel != specifiedinfo.specified_chan)
			{
				bss_info++;
				continue;
			}
		}
        j += sprintf(buf1 + j, "%d,", bss_info->channel);

        j += sprintf(buf1 + j, "%d,", bss_info->freq_offset);
		
        j += sprintf(buf1 + j, "%d,", bss_info->freqcal_val);	

        j += sprintf(buf1 + j, "%d,", bss_info->pairwise_cipher);	

        j += sprintf(buf1 + j, "%d,", bss_info->group_cipher);

        j += sprintf(buf1 + j, "%d,", bss_info->bgn);

        j += sprintf(buf1 + j, "%d)", bss_info->wps_support);
		



        printf("%s\n", buf1);

        bss_info ++;
    }

end:
    if(buf)
    {
        tls_mem_free(buf);
    }
    if(buf1)
    {
        tls_mem_free(buf1);
    }
}

int user_scan_specified(u8 *ssid,  u8 *mac,  u8 chan,  u8 scan_type,  u32 min_interval,  u32 max_interval)
{
	struct tls_wifi_scan_param_t wscan_param;
	
	memset(&specifiedinfo, 0, sizeof(specifiedinfo));
	specifiedinfo.specified_mask = 0;
	if (strlen((const char *)ssid) && (strlen((const char *)ssid) < 33))
	{
		memset(specifiedinfo.specified_ssid, 0, 33);
		strcpy((char *)specifiedinfo.specified_ssid, (char *)ssid);
		specifiedinfo.specified_ssid_len = strlen((const char *)ssid);
		specifiedinfo.specified_mask = 1;
	}
	else
	{
		specifiedinfo.specified_ssid[0] = '\0';
		specifiedinfo.specified_ssid_len = 0;
		specifiedinfo.specified_mask &= ~1;
	}

	if (strlen((const char *)mac) == 17)
	{
		memset(specifiedinfo.specified_bssid, 0, 17);
		strcpy((char *)specifiedinfo.specified_bssid, (char *)mac);
		specifiedinfo.specified_mask |= 2;
	}
	else
	{
		specifiedinfo.specified_bssid[0] = '\0';
		specifiedinfo.specified_mask &= ~2;
	}

	if (scan_type)
	{
		wscan_param.scan_type = 1;
	}
	else
	{
		wscan_param.scan_type = 0;
	}
	if (chan == 0)
	{
		specifiedinfo.specified_chan = 0;
		wscan_param.scan_chanlist = 0x1FFF;
		specifiedinfo.specified_mask &= ~4;
	}
	else
	{
		specifiedinfo.specified_chan = chan;
		specifiedinfo.specified_mask |=4;
		wscan_param.scan_chanlist = 1<<(chan-1);
	}

	if (min_interval == 0)
	{
		wscan_param.scan_chinterval = 120;
		wscan_param.scan_times = 1;
	}
	else
	{
		wscan_param.scan_chinterval = min_interval;
		if (max_interval >= min_interval)
		{
			wscan_param.scan_times = (max_interval + min_interval/2)/min_interval;
		}
		else
		{
			wscan_param.scan_times = 1;
		}
	}

	tls_wifi_scan_by_param(&wscan_param);
	
	return 0;
}

/*Scan demo*/
int scan_demo(void)
{
    tls_wifi_scan_result_cb_register(wifi_scan_handler);
    tls_wifi_scan();
    return WM_SUCCESS;
}

int scan_format2_demo(void)
{
	
    tls_wifi_scan_result_cb_register(wifi_scan_format2_handler);
    tls_wifi_scan();
    return WM_SUCCESS;
}

/*specified Scan demo*/
int scan_specified_demo(u8 *ssid,  u8 *mac,  u32 chan,  u32 scan_type,  u32 min_interval,  u32 max_interval)
{
    tls_wifi_scan_result_cb_register(wifi_scan_format2_filter_handler);
    user_scan_specified(ssid, mac, chan, scan_type, min_interval, max_interval);
    return WM_SUCCESS;
}


#endif

