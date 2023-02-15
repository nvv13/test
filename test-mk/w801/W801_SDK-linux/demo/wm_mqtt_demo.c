/*****************************************************************************
*
* File Name : wm_mqtt_demo.c
*
* Description: mqtt demo function
*
* Copyright (c) 2015 Winner Micro Electronic Design Co., Ltd.
* All rights reserved.
*
* Author : LiLimin
*
* Date : 2019-3-24
*****************************************************************************/
#include <string.h>
#include "list.h"
#include "wm_include.h"
#include "wm_netif.h"
#include "wm_demo.h"
#include "tls_common.h"
#include "lwip/netif.h"
#include "wm_sockets.h"
#include "lwip/inet.h"
#include "wm_sockets.h"
#include "libemqtt.h"
#include "private-libwebsockets.h"
#include "libwebsockets.h"
#include "HTTPClient.h"

#if DEMO_MQTT

#define MQTT_DEMO_TASK_PRIO             39
#define MQTT_DEMO_TASK_SIZE             2048
#define MQTT_DEMO_QUEUE_SIZE            16

#define MQTT_DEMO_RECV_BUF_LEN_MAX      1024

#define MQTT_DEMO_CMD_START             0x1
#define MQTT_DEMO_CMD_HEART             0x2
#define MQTT_DEMO_CMD_LOOP              0x3

#define MQTT_DEMO_READ_TIMEOUT        (-1000)

#define MQTT_DEMO_READ_TIME_SEC         5
#define MQTT_DEMO_READ_TIME_US          0

#define MQTT_DEMO_CLIENT_ID            "wm_mqtt_client"
#define MQTT_DEMO_TX_PUB_TOPIC         "topic_tx"
#define MQTT_DEMO_RX_PUB_TOPIC         "topic_rx"

#define MQTT_DEMO_SERVER_ADDR          "192.168.1.101"
#define MQTT_DEMO_SERVER_PORT_TCP           1883
#define MQTT_DEMO_SERVER_PORT_TLS           8883
#define MQTT_DEMO_SERVER_PORT_WS            8080
#define MQTT_DEMO_SERVER_PORT_WSS           8081

typedef enum _conn_type
{
	MQTT_CONN_TYPE_TCP = 0,
	MQTT_CONN_TYPE_TLS,
	MQTT_CONN_TYPE_WS,
	MQTT_CONN_TYPE_WSS,
}mqtt_conn_type;

typedef struct _mqtt_demo_context
{
	mqtt_conn_type conn_type;
	int mqtt_demo_socket_id;
	tls_ssl_t *ssl;
	struct lws* ws_context;
	struct dl_list ws_tx_list;
	struct dl_list ws_rx_list;
	uint16_t server_port;
	int (*connect)(struct _mqtt_demo_context *ctx, const struct sockaddr *name, socklen_t namelen,char *hostname);
	int (*close_mqtt)(struct _mqtt_demo_context *ctx);
	int (*send_packet)(int socket_info, const void *buf, unsigned int count);
	int (*read_packet)(struct _mqtt_demo_context *ctx, uint8_t *buf, int buf_len, int sec, int us);
	int mqtt_demo_mqtt_keepalive;
	tls_os_timer_t *mqtt_demo_heartbeat_timer;
	mqtt_broker_handle_t mqtt_demo_mqtt_broker;
	uint8_t mqtt_demo_packet_buffer[MQTT_DEMO_RECV_BUF_LEN_MAX];
}mqtt_demo_context_t;

static bool mqtt_demo_inited = FALSE;
static OS_STK mqtt_demo_task_stk[MQTT_DEMO_TASK_SIZE];
static tls_os_queue_t *mqtt_demo_task_queue = NULL;
/*static tls_os_timer_t *mqtt_demo_heartbeat_timer = NULL;

static int mqtt_demo_socket_id;
static int mqtt_demo_mqtt_keepalive = 300;
static mqtt_broker_handle_t mqtt_demo_mqtt_broker;

static uint8_t mqtt_demo_packet_buffer[MQTT_DEMO_RECV_BUF_LEN_MAX];*/

extern struct netif *tls_get_netif(void);
extern int wm_printf(const char *fmt, ...);

static void mqtt_demo_net_status(u8 status)
{
    struct netif *netif = tls_get_netif();

    switch(status)
    {
    case NETIF_WIFI_JOIN_FAILED:
        wm_printf("sta join net failed\n");
        break;
    case NETIF_WIFI_DISCONNECTED:
        wm_printf("sta net disconnected\n");
        break;
    case NETIF_IP_NET_UP:
        wm_printf("sta ip: %v\n", netif->ip_addr.addr);
        tls_os_queue_send(mqtt_demo_task_queue, (void *)MQTT_DEMO_CMD_START, 0);
        break;
    default:
        break;
    }
}

static void mqtt_demo_heart_timer(void *ptmr, void *parg)
{
    tls_os_status_t os_status = tls_os_queue_send(mqtt_demo_task_queue, (void *)MQTT_DEMO_CMD_HEART, 0);
	wm_printf("==> heart timer %d\n", os_status);
}

typedef struct _ws_packet
{
    struct dl_list list;
	char *buf;
	int buf_len;
}ws_packet;
static int lwsCallbackNotify(struct lws *wsi,enum lws_callback_reasons reason,void *user, void *in, size_t len)
{
	mqtt_demo_context_t *ctx = (mqtt_demo_context_t *)user;
	u32 cpu_sr;
	ws_packet *packet = NULL;
	//wm_printf("%s: enter ctx 0x%x, reason %d\n", __func__, ctx, reason);
	switch (reason) 
    {
    	case LWS_CALLBACK_CLIENT_ESTABLISHED:
    		wm_printf("CLIENT_ESTABLISHED\n");
    		//now_state = HANDLE_SESSION;
    		break;

    	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
    		wm_printf("CLIENT_CONNECTION_ERROR\n");
    		//now_state = EXIT_SESSION;
    		break;

    	case LWS_CALLBACK_CLOSED:
    		wm_printf("CLOSED\n");
    		//now_state = EXIT_SESSION;
    		break;

    	case LWS_CALLBACK_CLIENT_RECEIVE:
    		//((char *)in)[len] = '\0';
			wm_printf("lws receive len %d\n", len);
    		do
			{
				packet = (ws_packet *)tls_mem_alloc(sizeof(ws_packet));
				if(packet == NULL)
				{
					wm_printf("%s: malloc packet error\n", __func__);
					break;
				}
				dl_list_init(&packet->list);
				packet->buf = tls_mem_alloc(len);
				if(packet->buf == NULL)
				{
					tls_mem_free(packet);
					wm_printf("%s: malloc packet buffer error\n", __func__);
					break;
				}
				memset(packet->buf, 0, len);
				memcpy(packet->buf, in, len);
				packet->buf_len = len;
				cpu_sr = tls_os_set_critical();
				dl_list_add_tail(&ctx->ws_rx_list, &packet->list);
				tls_os_release_critical(cpu_sr);
				//wm_printf("recv packets count %d\n", dl_list_len(&ctx->ws_rx_list));
			}while(0);
    		break;

    	case LWS_CALLBACK_CLIENT_WRITEABLE:
			//wm_printf("lws writeable list count %d\n", dl_list_len(&ctx->ws_tx_list));
    		packet = dl_list_first(&ctx->ws_tx_list, ws_packet, list);
			//wm_printf("dl_list_first packet 0x%x\n", packet);
			if(packet != NULL)
			{
				wm_printf("lws_write ");
				int n = lws_write( wsi, (unsigned char *)packet->buf + LWS_SEND_BUFFER_PRE_PADDING, packet->buf_len, LWS_WRITE_BINARY );
				wm_printf("ret %d\n", n);
				if( n <= 0 )
				{
					wm_printf("send error %d\r\n", n);
                }
				cpu_sr = tls_os_set_critical();
				dl_list_del(&packet->list);
				tls_os_release_critical(cpu_sr);
				tls_mem_free(packet->buf);
				tls_mem_free(packet);
			}
    		break;

    	default:
    		break;
	}

	return 0;
}

static struct lws_protocols protocols[] = {
	{
		"mqtt",
		lwsCallbackNotify,
		0,
		2048,
		0,
		NULL
	},
	{ NULL, NULL, 0, 0, 0, NULL } /* end */
};
#define   LWS_SERVER_PATH    "/"

static int mqtt_ws_connect(mqtt_demo_context_t *ctx, const struct sockaddr *name, socklen_t namelen,char *hostname)
{
    struct lws_client_connect_info connInfo;
    struct lws_context_creation_info info;    
    struct lws_context *lwscontext = NULL;

	memset(&info, 0, sizeof(info) );
    info.port = CONTEXT_PORT_NO_LISTEN;
	if(ctx->server_port == MQTT_DEMO_SERVER_PORT_WSS)
	{
    	info.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
	}
	protocols[0].user = ctx;
	protocols[0].per_session_data_size = sizeof(mqtt_demo_context_t *);
	wm_printf("protocols[0].user 0x%x\n", protocols[0].user);
    info.protocols = protocols;
    info.max_http_header_data=512;
    info.max_http_header_pool=4;
    
    lwscontext = lws_create_context(&info);
    memset(&connInfo, 0 , sizeof(connInfo) );
    connInfo.context = lwscontext;
    connInfo.address = MQTT_DEMO_SERVER_ADDR;
    connInfo.port = ctx->server_port;
    connInfo.ssl_connection = ctx->server_port == MQTT_DEMO_SERVER_PORT_WSS ? 1 : 0;
    connInfo.path = LWS_SERVER_PATH;
    connInfo.host = MQTT_DEMO_SERVER_ADDR;
    connInfo.userdata = ctx;
    connInfo.protocol = protocols[0].name;
    connInfo.ietf_version_or_minus_one=13;
    connInfo.origin = MQTT_DEMO_SERVER_ADDR;
	ctx->ws_context = lws_client_connect_via_info( &connInfo );
    if(ctx->ws_context == NULL )
	{
		lws_context_destroy(lwscontext);
		ctx->ws_context->context = NULL;
		return -1;
    }
	return 0;
}
static int mqtt_ws_close_socket(mqtt_demo_context_t *ctx)
{
	wm_printf("EXIT_SESSION ctx 0x%x\n", ctx);
	if(ctx->ws_context != NULL )
	{
		if( ctx->ws_context->context != NULL ) {
		    lws_context_destroy(ctx->ws_context->context);
			ctx->ws_context->context = NULL;
		}
		lws_free(ctx->ws_context);
	    ctx->ws_context = NULL;
	}
	return 0;
}
static int mqtt_ws_send_packet(int socket_info, const void *buf, unsigned int count)
{
    mqtt_demo_context_t *ctx = (mqtt_demo_context_t *)socket_info;
	u32 cpu_sr;
	ws_packet *packet = (ws_packet *)tls_mem_alloc(sizeof(ws_packet));
	
	//wm_printf("%s: enter ctx 0x%x\n", __func__, ctx);
	if(packet == NULL)
	{
		wm_printf("%s: malloc packet error\n", __func__);
		return -1;
	}
	dl_list_init(&packet->list);
	packet->buf = tls_mem_alloc(count + LWS_SEND_BUFFER_PRE_PADDING + 4);
	if(packet->buf == NULL)
	{
		tls_mem_free(packet);
		wm_printf("%s: malloc packet buffer error\n", __func__);
		return -1;
	}
	memset(packet->buf, 0, count + LWS_SEND_BUFFER_PRE_PADDING + 4);
	memcpy(packet->buf + LWS_SEND_BUFFER_PRE_PADDING, buf, count);
	packet->buf_len = count;
	cpu_sr = tls_os_set_critical();
	dl_list_add_tail(&ctx->ws_tx_list, &packet->list);
	tls_os_release_critical(cpu_sr);
	//wm_printf("send packets count %d\n", dl_list_len(&ctx->ws_tx_list));
	lws_callback_on_writable(ctx->ws_context);
	return count;
}
static int mqtt_ws_read_packet(mqtt_demo_context_t *ctx, uint8_t *buf, int buf_len, int sec, int us)
{
	u32 cpu_sr;
	ws_packet *packet = NULL;
	int start_tick = tls_os_get_time();
	volatile int now_tick;
	int timeout = (sec*1000 + us/1000) * HZ / 1000;
	//wm_printf("%s: enter ctx 0x%x, timeout %d, start_tick %d\n", __func__, ctx, timeout, start_tick);
	
	do
	{
		lws_service(ctx->ws_context->context, 250);
		lws_callback_on_writable(ctx->ws_context);
		packet = dl_list_first(&ctx->ws_rx_list, ws_packet, list);
		//wm_printf("%s dl_list_first packet 0x%x\n", __func__, packet);
		if(packet != NULL)
		{
			if(buf_len > packet->buf_len)
			{
				buf_len = packet->buf_len;
			}
			memcpy(buf, packet->buf, buf_len);
			cpu_sr = tls_os_set_critical();
			dl_list_del(&packet->list);
			tls_os_release_critical(cpu_sr);
			tls_mem_free(packet->buf);
			tls_mem_free(packet);
			return buf_len;
		}
		tls_os_time_delay(10);
		now_tick = tls_os_get_time();
		//wm_printf("%s tick now %d | %d < %d\n", __func__, now_tick, (now_tick - start_tick), timeout);
	}while((now_tick - start_tick) < timeout);
	//wm_printf("%s return timeout\n", __func__);
	return MQTT_DEMO_READ_TIMEOUT;
}

static int mqtt_tls_connect(mqtt_demo_context_t *ctx, const struct sockaddr *name, socklen_t namelen,char *hostname)
{
	return HTTPWrapperSSLConnect(&ctx->ssl, ctx->mqtt_demo_socket_id, name, namelen, hostname);
}
static int mqtt_tls_close_socket(mqtt_demo_context_t *ctx)
{
	return HTTPWrapperSSLClose(ctx->ssl, ctx->mqtt_demo_socket_id);
}
static int mqtt_tls_send_packet(int socket_info, const void *buf, unsigned int count)
{
    mqtt_demo_context_t *ctx = (mqtt_demo_context_t *)socket_info;
	return HTTPWrapperSSLSend(ctx->ssl, ctx->mqtt_demo_socket_id, (char *)buf, count, 0);
}
static int mqtt_tls_read_packet(mqtt_demo_context_t *ctx, uint8_t *buf, int buf_len, int sec, int us)
{
	int ret = HTTPWrapperSSLRecv(ctx->ssl, ctx->mqtt_demo_socket_id, (char *)buf, buf_len, 0);
    if (ret <= 0)
    {
        if (SOCKET_SSL_MORE_DATA == ret)
        {
            ret = buf_len;
        }
        else if(MBEDTLS_ERR_SSL_TIMEOUT == ret)
        {
        	ret = MQTT_DEMO_READ_TIMEOUT;
        }
		else
        {
            wm_printf("tls recv %d closed\r\n", ret);
        }
    }
    return ret;
}

static int mqtt_tcp_connect(mqtt_demo_context_t *ctx, const struct sockaddr *name, socklen_t namelen,char *hostname)
{
	return connect(ctx->mqtt_demo_socket_id, name, namelen);
}
static int mqtt_tcp_close_socket(mqtt_demo_context_t *ctx)
{
    int fd = ctx->mqtt_demo_socket_id;
    return closesocket(fd);
}
static int mqtt_tcp_send_packet(int socket_info, const void *buf, unsigned int count)
{
    mqtt_demo_context_t *ctx = (mqtt_demo_context_t *)socket_info;
    return send(ctx->mqtt_demo_socket_id, buf, count, 0);
}
static int mqtt_tcp_read_packet(mqtt_demo_context_t *ctx, uint8_t *buf, int buf_len, int sec, int us)
{
    int ret = 0;
	int bytes_rcvd;
	int fd = ctx->mqtt_demo_socket_id;

    if ((sec >= 0) || (us >= 0))
    {
        fd_set readfds;
        struct timeval tmv;

        // Initialize the file descriptor set
        FD_ZERO (&readfds);
        FD_SET (fd, &readfds);

        // Initialize the timeout data structure
        tmv.tv_sec = sec;
        tmv.tv_usec = us;

        // select returns 0 if timeout, 1 if input available, -1 if error
        ret = select(fd + 1, &readfds, NULL, NULL, &tmv);
        if(ret < 0)
            return -2;
        else if(ret == 0)
            return MQTT_DEMO_READ_TIMEOUT;

    }

    if((bytes_rcvd = recv(fd, buf, buf_len, 0)) <= 0)
    {
        //printf("%d, %d\r\n", bytes_rcvd, mqtt_demo_socket_id);
        return -1;
    }
	return bytes_rcvd;
}

static int mqtt_demo_recv_parse(mqtt_demo_context_t *ctx, int sec, int us)
{
    int total_bytes = 0, bytes_rcvd, packet_length;
    memset(ctx->mqtt_demo_packet_buffer, 0, sizeof(ctx->mqtt_demo_packet_buffer));

	bytes_rcvd = ctx->read_packet(ctx, (ctx->mqtt_demo_packet_buffer + total_bytes), MQTT_DEMO_RECV_BUF_LEN_MAX, sec, us);
	if(bytes_rcvd <= 0)
	{
		return bytes_rcvd;
	}
    //printf("recv [len=%d] : %s\n", bytes_rcvd, mqtt_demo_packet_buffer);
    total_bytes += bytes_rcvd; // Keep tally of total bytes
    if (total_bytes < 2)
        return -1;

    // now we have the full fixed header in mqtt_demo_packet_buffer
    // parse it for remaining length and number of bytes
    uint16_t rem_len = mqtt_parse_rem_len(ctx->mqtt_demo_packet_buffer);
    uint8_t rem_len_bytes = mqtt_num_rem_len_bytes(ctx->mqtt_demo_packet_buffer);

    //packet_length = mqtt_demo_packet_buffer[1] + 2; // Remaining length + fixed header length
    // total packet length = remaining length + byte 1 of fixed header + remaning length part of fixed header
    packet_length = rem_len + rem_len_bytes + 1;

    while(total_bytes < packet_length) // Reading the packet
    {
        if((bytes_rcvd = ctx->read_packet(ctx, (ctx->mqtt_demo_packet_buffer + total_bytes), MQTT_DEMO_RECV_BUF_LEN_MAX, sec, us)) <= 0)
            return -1;
        total_bytes += bytes_rcvd; // Keep tally of total bytes
    }

    return packet_length;
}

static int mqtt_demo_init_socket(mqtt_demo_context_t *ctx, const char *hostname, short port, int keepalive)
{
    int flag = 1;
    struct hostent *hp;

    // Create the socket
    if((ctx->mqtt_demo_socket_id = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    // Disable Nagle Algorithm
    if (setsockopt(ctx->mqtt_demo_socket_id, IPPROTO_TCP, 0x01, (char *)&flag, sizeof(flag)) < 0)
    {
        ctx->close_mqtt(ctx);
        return -2;
    }

    // query host ip start
    hp = gethostbyname(hostname);
    if (hp == NULL )
    {
        ctx->close_mqtt(ctx);
        return -2;
    }

    struct sockaddr_in socket_address;
    memset(&socket_address, 0, sizeof(struct sockaddr_in));
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(port);
    memcpy(&(socket_address.sin_addr), hp->h_addr, hp->h_length);

    // Connect the socket
    if((ctx->connect(ctx, (struct sockaddr *)&socket_address, sizeof(socket_address), NULL)) < 0)
    {
        ctx->close_mqtt(ctx);
        return -1;
    }

    // MQTT stuffs
    mqtt_set_alive(&ctx->mqtt_demo_mqtt_broker, ctx->mqtt_demo_mqtt_keepalive);
    ctx->mqtt_demo_mqtt_broker.socketid = (int)ctx;
    ctx->mqtt_demo_mqtt_broker.mqttsend = ctx->send_packet;
    //wm_printf("socket id = %d\n", mqtt_demo_socket_id);
    return 0;
}

static int mqtt_demo_init(mqtt_demo_context_t *ctx)
{
    int packet_length, ret = 0;
    uint16_t msg_id, msg_id_rcv;

    wm_printf("step1: init mqtt lib.\r\n");
    mqtt_init(&ctx->mqtt_demo_mqtt_broker, MQTT_DEMO_CLIENT_ID);
	mqtt_init_auth(&ctx->mqtt_demo_mqtt_broker, "admin", "password");

    wm_printf("step2: establishing TCP connection.\r\n");
    ret = mqtt_demo_init_socket(ctx, MQTT_DEMO_SERVER_ADDR, ctx->server_port, ctx->mqtt_demo_mqtt_keepalive);
    if(ret)
    {
        wm_printf("init_socket ret=%d\n", ret);
        return -4;
    }

    wm_printf("step3: establishing mqtt connection.\r\n");
    ret = mqtt_connect(&ctx->mqtt_demo_mqtt_broker);
    if(ret)
    {
        wm_printf("mqtt_connect ret=%d\n", ret);
        return -5;
    }

    packet_length = mqtt_demo_recv_parse(ctx, MQTT_DEMO_READ_TIME_SEC, MQTT_DEMO_READ_TIME_US);
    if(packet_length < 0)
    {
        wm_printf("Error(%d) on read packet!\n", packet_length);
        ctx->close_mqtt(ctx);
        return -1;
    }

    if(MQTTParseMessageType(ctx->mqtt_demo_packet_buffer) != MQTT_MSG_CONNACK)
    {
        wm_printf("CONNACK expected!\n");
        ctx->close_mqtt(ctx);
        return -2;
    }

    if(ctx->mqtt_demo_packet_buffer[3] != 0x00)
    {
        wm_printf("CONNACK failed!\n");
        ctx->close_mqtt(ctx);
        return -2;
    }


    wm_printf("step4: subscribe mqtt\r\n");
    mqtt_subscribe(&ctx->mqtt_demo_mqtt_broker, MQTT_DEMO_TX_PUB_TOPIC, &msg_id);

    packet_length = mqtt_demo_recv_parse(ctx, MQTT_DEMO_READ_TIME_SEC, MQTT_DEMO_READ_TIME_US);
    if(packet_length < 0)
    {
        wm_printf("Error(%d) on read packet!\n", packet_length);
        ctx->close_mqtt(ctx);
        return -1;
    }

    if(MQTTParseMessageType(ctx->mqtt_demo_packet_buffer) != MQTT_MSG_SUBACK)
    {
        wm_printf("SUBACK expected!\n");
        ctx->close_mqtt(ctx);
        return -2;
    }

    msg_id_rcv = mqtt_parse_msg_id(ctx->mqtt_demo_packet_buffer);
    if(msg_id != msg_id_rcv)
    {
        wm_printf("%d message id was expected, but %d message id was found!\n", msg_id, msg_id_rcv);
        ctx->close_mqtt(ctx);
        return -3;
    }
#if 0
	wm_printf("step4+: unsubscribe mqtt\r\n");
	mqtt_unsubscribe(&ctx->mqtt_demo_mqtt_broker, MQTT_DEMO_TX_PUB_TOPIC, &msg_id);

	packet_length = mqtt_demo_recv_parse(ctx, MQTT_DEMO_READ_TIME_SEC, MQTT_DEMO_READ_TIME_US);
	if(packet_length < 0)
	{
		wm_printf("Error(%d) on read packet!\n", packet_length);
		ctx->close_mqtt(ctx);
		return -1;
	}

	if(MQTTParseMessageType(ctx->mqtt_demo_packet_buffer) != MQTT_MSG_UNSUBACK)
	{
		wm_printf("UNSUBACK expected!\n");
		ctx->close_mqtt(ctx);
		return -2;
	}
#endif
    wm_printf("step5: start the Heart-beat preservation timer ");
    ret = tls_os_timer_create(&ctx->mqtt_demo_heartbeat_timer,
                              mqtt_demo_heart_timer,
                              NULL, (10 * HZ), TRUE, NULL);
    if (TLS_OS_SUCCESS == ret)
	{
		tls_os_timer_start(ctx->mqtt_demo_heartbeat_timer);
	}
	wm_printf("%s\r\n" , TLS_OS_SUCCESS == ret ? "ok" : "error");

    /* step6: push mqtt subscription (when a subscription message is received) */

    return 0;
}

static int mqtt_demo_loop(mqtt_demo_context_t *ctx)
{
    int packet_length = 0;
    int counter = 0;

    counter++;
    packet_length = mqtt_demo_recv_parse(ctx, 1, 1);
    if(packet_length > 0)
    {
        //wm_printf("recvd Packet Header: 0x%x...\n", mqtt_demo_packet_buffer[0]);

        if (MQTTParseMessageType(ctx->mqtt_demo_packet_buffer) == MQTT_MSG_PUBLISH)
        {
            uint8_t topic[100], *msg;
            uint16_t len;
            len = mqtt_parse_pub_topic(ctx->mqtt_demo_packet_buffer, topic);
            topic[len] = '\0'; // for printf
            len = mqtt_parse_publish_msg(ctx->mqtt_demo_packet_buffer, &msg);
            msg[len] = '\0'; // for printf
            wm_printf("recvd: %s >>> %s\n", topic, msg);

            mqtt_publish(&ctx->mqtt_demo_mqtt_broker, (const char *)MQTT_DEMO_RX_PUB_TOPIC, (const char *)msg, len, 0);
            wm_printf("pushed: %s <<< %s\n", MQTT_DEMO_RX_PUB_TOPIC, msg);
        }

        tls_os_queue_send(mqtt_demo_task_queue, (void *)MQTT_DEMO_CMD_LOOP, 0);
    }
    else if (packet_length == MQTT_DEMO_READ_TIMEOUT)
    {
        tls_os_queue_send(mqtt_demo_task_queue, (void *)MQTT_DEMO_CMD_LOOP, 0);
    }
    else if(packet_length == -1)
    {
        wm_printf("mqtt error:(%d), stop mqtt demo!\n", packet_length);
        tls_os_timer_stop(ctx->mqtt_demo_heartbeat_timer);
        ctx->close_mqtt(ctx);
    }

    return 0;
}

static void mqtt_demo_task(void *p)
{
    int ret;
    void *msg;
    struct tls_ethif *ether_if = tls_netif_get_ethif();
	mqtt_demo_context_t *ctx = tls_mem_alloc(sizeof(mqtt_demo_context_t));

	if(ctx == NULL)
	{
		wm_printf("MQTT Demo context malloc Error!\n");
		return;
	}
	memset(ctx, 0, sizeof(mqtt_demo_context_t));
	ctx->conn_type = (mqtt_conn_type)p;
	wm_printf("conn_type %u ctx 0x%x mqtt_demo_socket_id 0x%x\n", ctx->conn_type, ctx, (int)&ctx->mqtt_demo_socket_id);
	switch(ctx->conn_type)
	{
		case MQTT_CONN_TYPE_TCP:
			ctx->connect = mqtt_tcp_connect;
			ctx->close_mqtt = mqtt_tcp_close_socket;
			ctx->send_packet = mqtt_tcp_send_packet;
			ctx->read_packet = mqtt_tcp_read_packet;
			ctx->server_port = MQTT_DEMO_SERVER_PORT_TCP;
			break;
		case MQTT_CONN_TYPE_TLS:
			ctx->connect = mqtt_tls_connect;
			ctx->close_mqtt = mqtt_tls_close_socket;
			ctx->send_packet = mqtt_tls_send_packet;
			ctx->read_packet = mqtt_tls_read_packet;
			ctx->server_port = MQTT_DEMO_SERVER_PORT_TLS;
			break;
		case MQTT_CONN_TYPE_WS:
		case MQTT_CONN_TYPE_WSS:
			ctx->connect = mqtt_ws_connect;
			ctx->close_mqtt = mqtt_ws_close_socket;
			ctx->send_packet = mqtt_ws_send_packet;
			ctx->read_packet = mqtt_ws_read_packet;
			ctx->server_port = MQTT_CONN_TYPE_WS == ctx->conn_type ? MQTT_DEMO_SERVER_PORT_WS : MQTT_DEMO_SERVER_PORT_WSS;
			break;
		default:
			wm_printf("conn_type %u is not supported error!\n");
			return;
	}
	ctx->mqtt_demo_socket_id = -1;
	ctx->mqtt_demo_mqtt_keepalive = 300;
	dl_list_init(&ctx->ws_tx_list);
	dl_list_init(&ctx->ws_rx_list);
    if (ether_if->status)
    {
        wm_printf("sta ip: %v\n", ether_if->ip_addr.addr);
        tls_os_queue_send(mqtt_demo_task_queue, (void *)MQTT_DEMO_CMD_START, 0);
    }

    for ( ; ; )
    {
        ret = tls_os_queue_receive(mqtt_demo_task_queue, (void **)&msg, 0, 0);
        if (!ret)
        {
            switch((u32)msg)
            {
            case MQTT_DEMO_CMD_START:
                do
                {
                    ret = mqtt_demo_init(ctx);
                    if (ret)
                        break;
                    tls_os_queue_send(mqtt_demo_task_queue, (void *)MQTT_DEMO_CMD_LOOP, 0);
                }
                while (0);
                break;
            case MQTT_DEMO_CMD_HEART:
                wm_printf("send heart ping\r\n");
                mqtt_ping(&ctx->mqtt_demo_mqtt_broker);
                break;
            case MQTT_DEMO_CMD_LOOP:
                mqtt_demo_loop(ctx);
                break;
            default:
                break;
            }
        }
    }
}


//mqtt demo
//测试服务器:mqtt.yichen.link:3883
//服务器端用于发送的订阅主题为:winnermicro/mqtt_tx_demo
//服务器端用于接收的订阅主题为:winnermicro/mqtt_rx_demo
//工作流程: 接收到winnermicro/mqtt_tx_demo推送的消息后打印在屏幕上，并再次推送到winnermicro/mqtt_rx_demo
int mqtt_demo(int type)
{
    if (!mqtt_demo_inited)
    {
        tls_os_task_create(NULL, NULL, mqtt_demo_task,
                           (void *)type, (void *)mqtt_demo_task_stk,  /* task's stack start address */
                           MQTT_DEMO_TASK_SIZE * sizeof(u32), /* task's stack size, unit:byte */
                           MQTT_DEMO_TASK_PRIO, 0);

        tls_os_queue_create(&mqtt_demo_task_queue, MQTT_DEMO_QUEUE_SIZE);

        tls_netif_add_status_event(mqtt_demo_net_status);

        mqtt_demo_inited = TRUE;
    }

    return WM_SUCCESS;
}

#endif

