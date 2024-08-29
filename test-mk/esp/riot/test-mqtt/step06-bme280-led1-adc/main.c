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
#include "mutex.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"



#include <stdlib.h>
#include <stdio.h>
#include "periph/gpio.h"

#include "periph/adc.h"

//#define BMX280_PARAM_I2C_ADDR       (0x76)
//https://gitlab.com/gschorcht/RIOT.wiki-Images/-/tree/master/esp8266

#include "bmx280_params.h"
#include "bmx280.h"
#include "xtimer.h"
#include "fmt.h"


#include "thread.h"
char stack[THREAD_STACKSIZE_MAIN];

#define SAMPLING_PERIOD     2

#define BUF_SIZE                        1024
#define MQTT_VERSION_v311               4       /* MQTT v3.1.1 version is 4 */
#define COMMAND_TIMEOUT_MS              4000

#ifndef DEFAULT_MQTT_CLIENT_ID
#define DEFAULT_MQTT_CLIENT_ID          ""
#endif

#ifndef DEFAULT_MQTT_USER
#define DEFAULT_MQTT_USER               ""
#endif

#ifndef DEFAULT_MQTT_PWD
#define DEFAULT_MQTT_PWD                ""
#endif

/**
 * @brief Default MQTT port
 */
#define DEFAULT_MQTT_PORT               1883

/**
 * @brief Keepalive timeout in seconds
 */
#define DEFAULT_KEEPALIVE_SEC           180

#ifndef MAX_LEN_TOPIC
#define MAX_LEN_TOPIC                   100
#endif

#ifndef MAX_TOPICS
#define MAX_TOPICS                      4
#endif

#define IS_CLEAN_SESSION                1
#define IS_RETAINED_MSG                 0

static MQTTClient client;
static Network network;
static int topic_cnt = 0;
static char _topic_to_subscribe[MAX_TOPICS][MAX_LEN_TOPIC];


bmx280_t dev;
gpio_t pin02;
gpio_t pin16;

adc_t  adc_line0;
int adc_line0_init;
int32_t adc_line0_sample;

static unsigned get_qos(const char *str)
{
    int qos = atoi(str);

    switch (qos) {
    case 1:     return QOS1;
    case 2:     return QOS2;
    default:    return QOS0;
    }
}

static void _on_msg_received(MessageData *data)
{
    printf("paho_mqtt: message received on topic"
           " %.*s: %.*s\n",
           (int)data->topicName->lenstring.len,
           data->topicName->lenstring.data, (int)data->message->payloadlen,
           (char *)data->message->payload);

    if(strncmp(data->topicName->lenstring.data, "/esp1/temp",(int)data->topicName->lenstring.len)==0)
     {
     gpio_toggle(pin02); //LED on Board
     }

    if(strncmp(data->topicName->lenstring.data, "/esp1/led1",(int)data->topicName->lenstring.len)==0)
     {
     if(strncmp(data->message->payload, "1",(int)data->message->payloadlen)==0)
      gpio_write(pin16,1);
      else
      gpio_write(pin16,0);
     }
               
}

static int _cmd_discon(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    topic_cnt = 0;
    int res = MQTTDisconnect(&client);
    if (res < 0) {
        printf("mqtt_example: Unable to disconnect\n");
    }
    else {
        printf("mqtt_example: Disconnect successful\n");
    }

    NetworkDisconnect(&network);
    return res;
}

static int _cmd_con(int argc, char **argv)
{
    if (argc < 2) {
        printf(
            "usage: %s <brokerip addr> [port] [clientID] [user] [password] "
            "[keepalivetime]\n",
            argv[0]);
        return 1;
    }

    char *remote_ip = argv[1];

    int ret = -1;

    /* ensure client isn't connected in case of a new connection */
    if (client.isconnected) {
        printf("mqtt_example: client already connected, disconnecting it\n");
        MQTTDisconnect(&client);
        NetworkDisconnect(&network);
    }

    int port = DEFAULT_MQTT_PORT;
    if (argc > 2) {
        port = atoi(argv[2]);
    }

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = MQTT_VERSION_v311;

    data.clientID.cstring = DEFAULT_MQTT_CLIENT_ID;
    if (argc > 3) {
        data.clientID.cstring = argv[3];
    }

    data.username.cstring = DEFAULT_MQTT_USER;
    if (argc > 4) {
        data.username.cstring = argv[4];
    }

    data.password.cstring = DEFAULT_MQTT_PWD;
    if (argc > 5) {
        data.password.cstring = argv[5];
    }

    data.keepAliveInterval = DEFAULT_KEEPALIVE_SEC;
    if (argc > 6) {
        data.keepAliveInterval = atoi(argv[6]);
    }

    data.cleansession = IS_CLEAN_SESSION;
    data.willFlag = 0;

    printf("mqtt_example: Connecting to MQTT Broker from %s %d\n",
            remote_ip, port);
    printf("mqtt_example: Trying to connect to %s , port: %d\n",
            remote_ip, port);
    ret = NetworkConnect(&network, remote_ip, port);
    if (ret < 0) {
        printf("mqtt_example: Unable to connect\n");
        return ret;
    }

    printf("user:%s clientId:%s password:%s\n", data.username.cstring,
             data.clientID.cstring, data.password.cstring);
    ret = MQTTConnect(&client, &data);
    if (ret < 0) {
        printf("mqtt_example: Unable to connect client %d\n", ret);
        _cmd_discon(0, NULL);
        return ret;
    }
    else {
        printf("mqtt_example: Connection successfully\n");
    }

    return (ret > 0) ? 0 : 1;
}

static int _cmd_pub(int argc, char **argv)
{
    enum QoS qos = QOS0;

    if (argc < 3) {
        printf("usage: %s <topic name> <string msg> [QoS level]\n",
               argv[0]);
        return 1;
    }
    if (argc == 4) {
        qos = get_qos(argv[3]);
    }
    MQTTMessage message;
    message.qos = qos;
    message.retained = IS_RETAINED_MSG;
    message.payload = argv[2];
    message.payloadlen = strlen(message.payload);

    int rc;
    if ((rc = MQTTPublish(&client, argv[1], &message)) < 0) {
        printf("mqtt_example: Unable to publish (%d)\n", rc);
    }
    else {
        printf("mqtt_example: Message (%s) has been published to topic %s"
               "with QOS %d\n",
               (char *)message.payload, argv[1], (int)message.qos);
    }

    return rc;
}

static int _cmd_sub(int argc, char **argv)
{
    enum QoS qos = QOS0;

    if (argc < 2) {
        printf("usage: %s <topic name> [QoS level]\n", argv[0]);
        return 1;
    }

    if (argc >= 3) {
        qos = get_qos(argv[2]);
    }

    if (topic_cnt > MAX_TOPICS) {
        printf("mqtt_example: Already subscribed to max %d topics,"
                "call 'unsub' command\n", topic_cnt);
        return -1;
    }

    if (strlen(argv[1]) > MAX_LEN_TOPIC) {
        printf("mqtt_example: Not subscribing, topic too long %s\n", argv[1]);
        return -1;
    }
    strncpy(_topic_to_subscribe[topic_cnt], argv[1], strlen(argv[1]));

    printf("mqtt_example: Subscribing to %s\n", _topic_to_subscribe[topic_cnt]);
    int ret = MQTTSubscribe(&client,
              _topic_to_subscribe[topic_cnt], qos, _on_msg_received);
    if (ret < 0) {
        printf("mqtt_example: Unable to subscribe to %s (%d)\n",
               _topic_to_subscribe[topic_cnt], ret);
        _cmd_discon(0, NULL);
    }
    else {
        printf("mqtt_example: Now subscribed to %s, QOS %d\n",
               argv[1], (int) qos);
        topic_cnt++;
    }
    return ret;
}

static int _cmd_unsub(int argc, char **argv)
{
    if (argc < 2) {
        printf("usage %s <topic name>\n", argv[0]);
        return 1;
    }

    int ret = MQTTUnsubscribe(&client, argv[1]);

    if (ret < 0) {
        printf("mqtt_example: Unable to unsubscribe from topic: %s\n", argv[1]);
        _cmd_discon(0, NULL);
    }
    else {
        printf("mqtt_example: Unsubscribed from topic:%s\n", argv[1]);
        topic_cnt--;
    }
    return ret;
}



char* a_con[]={"con","192.168.1.47","1883","cl1","esp1","EsP1pass123"};

char* a_sub_temp2[]={"sub","/esp1/temp"};
char* a_sub_led1[] ={"sub","/esp1/led1"};


char* a_temp2[]={"pub","/esp1/temp",NULL};
char* a_pressure2[]={"pub","/esp1/pressure",NULL};
char* a_humidity2[]={"pub","/esp1/humidity",NULL};
char* a_adc2[]={"pub","/esp1/adc",NULL};

char str_temp[8];
char str_hum[8];
char str_t[20];

size_t len;


static int _cmd_readtemp(int argc, char **argv)
{

       
       if(adc_line0_init==0)
        {
        adc_line0_sample=adc_sample(adc_line0,ADC_RES_10BIT);
        // printf("adc: %d\n", adc_line0_sample);

           str_t[0]=0;
           len = sprintf(str_t,"%d", adc_line0_sample);
           str_t[len] = 0;
           a_adc2[2]=str_t;
           _cmd_pub(3, a_adc2);
           xtimer_sleep(1);
        }

//    puts("\n+--------Starting Measurements--------+");

        /* read temperature, pressure [and humidity] values */
        int16_t temperature = bmx280_read_temperature(&dev);
        uint32_t pressure = bmx280_read_pressure(&dev);
#if defined(MODULE_BME280_SPI) || defined(MODULE_BME280_I2C)
        uint16_t humidity = bme280_read_humidity(&dev);
#endif

        /* format values for printing */
        len = fmt_s16_dfp(str_temp, temperature, -2);
        str_temp[len] = '\0';
#if defined(MODULE_BME280_SPI) || defined(MODULE_BME280_I2C)
        len = fmt_s16_dfp(str_hum, humidity, -2);
        str_hum[len] = '\0';
#endif


        /* print values to STDIO */
//        printf("Temperature [°C]: %s\n", str_temp);
//        printf("   Pressure [Pa]: %" PRIu32 "\n", pressure);

           str_t[0]=0;
           len = sprintf(str_t,"%s", str_temp);
           str_t[len] = 0;
            
           a_temp2[2]=str_t;
           if(_cmd_pub(3, a_temp2))
            {
            _cmd_con(6, a_con);
            _cmd_sub(2, a_sub_temp2);
            _cmd_sub(2, a_sub_led1);
            };

           xtimer_sleep(1);

           str_t[0]=0;
           len = sprintf(str_t,"%d.%02d", pressure / 133, pressure % 133);
           str_t[len] = 0;
           a_pressure2[2]=str_t;
           _cmd_pub(3, a_pressure2);

#if defined(MODULE_BME280_SPI) || defined(MODULE_BME280_I2C)
     xtimer_sleep(1);

//        printf("  Humidity [%%rH]: %s\n", str_hum);
           str_t[0]=0;
           sprintf(str_t,"%s\n", str_hum);
           a_humidity2[2]=str_t;
           _cmd_pub(3,a_humidity2);

#endif


    return 0;
    

}

void *thread_handler(void *arg)
{
    /* ... */
    (void)arg;

    char* a0[]={"temp"};

    while(true)
     {
     //gpio_toggle(pin);

     _cmd_readtemp(1, a0);

     xtimer_sleep(2);
     }

    return NULL;
}


static const shell_command_t shell_commands[] =
{
    { "con",    "connect to MQTT broker",             _cmd_con    },
    { "discon", "disconnect from the current broker", _cmd_discon },
    { "pub",    "publish something",                  _cmd_pub    },
    { "sub",    "subscribe topic",                    _cmd_sub    },
    { "unsub",  "unsubscribe from topic",             _cmd_unsub  },
    { "temp",   "read temperature",                  _cmd_readtemp  },
    { NULL,     NULL,                                 NULL        }
};

static unsigned char buf[BUF_SIZE];
static unsigned char readbuf[BUF_SIZE];

int main(void)
{

    xtimer_sleep(1);

    pin02 = GPIO_PIN(0,2);
    gpio_init(pin02,GPIO_OUT);
    
    pin16 = GPIO_PIN(0,16);
    gpio_init(pin16,GPIO_OUT);


    adc_line0_init=adc_init(adc_line0);
    if(adc_line0_init==0)
     puts("ADC init OK\n");
     else
     puts("ADC init ERROR\n");
    
    
    

    puts("BMX280 test application\n");

#ifdef BMX280_USE_SPI
    puts("ifdef BMX280_USE_SPI\n");
#endif

    puts("+------------Initializing------------+");
    switch (bmx280_init(&dev, &bmx280_params[0])) {
        case BMX280_ERR_BUS:
            puts("[Error] Something went wrong when using the I2C bus");
            return 1;
        case BMX280_ERR_NODEV:
            puts("[Error] Unable to communicate with any BMX280 device");
            return 1;
        default:
            /* all good -> do nothing */
            break;
    }

    puts("Initialization successful\n");
    

    puts("+------------Calibration Data------------+");
    printf("dig_T1: %u\n", dev.calibration.dig_T1);
    printf("dig_T2: %i\n", dev.calibration.dig_T2);
    printf("dig_T3: %i\n", dev.calibration.dig_T3);

    printf("dig_P1: %u\n", dev.calibration.dig_P1);
    printf("dig_P2: %i\n", dev.calibration.dig_P2);
    printf("dig_P3: %i\n", dev.calibration.dig_P3);
    printf("dig_P4: %i\n", dev.calibration.dig_P4);
    printf("dig_P5: %i\n", dev.calibration.dig_P5);
    printf("dig_P6: %i\n", dev.calibration.dig_P6);
    printf("dig_P7: %i\n", dev.calibration.dig_P7);
    printf("dig_P8: %i\n", dev.calibration.dig_P8);
    printf("dig_P9: %i\n", dev.calibration.dig_P9);

#if defined(MODULE_BME280_SPI) || defined(MODULE_BME280_I2C)
    printf("dig_H1: %u\n", dev.calibration.dig_H1);
    printf("dig_H2: %i\n", dev.calibration.dig_H2);
    printf("dig_H3: %i\n", dev.calibration.dig_H3);
    printf("dig_H4: %i\n", dev.calibration.dig_H4);
    printf("dig_H5: %i\n", dev.calibration.dig_H5);
    printf("dig_H6: %i\n", dev.calibration.dig_H6);
#endif

    
#ifdef MODULE_LWIP
    /* let LWIP initialize */
    xtimer_sleep(1);
#endif

    NetworkInit(&network);

    MQTTClientInit(&client, &network, COMMAND_TIMEOUT_MS, buf, BUF_SIZE,
                   readbuf,
                   BUF_SIZE);
    printf("Running mqtt paho example. Type help for commands info\n");

    MQTTStartTask(&client);

    xtimer_sleep(1);
    _cmd_con(6, a_con);
    _cmd_sub(2, a_sub_temp2);
    _cmd_sub(2, a_sub_led1);
    
    thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  thread_handler, NULL,
                  "thread");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}
