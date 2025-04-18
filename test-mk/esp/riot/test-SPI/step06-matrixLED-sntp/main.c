/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup tests
 * @{
 *
 * @file
 * @brief       Application for testing low-level SPI driver implementations
 *
 * This implementation covers both, master and slave configurations.
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include "irq.h"

#include "xtimer.h"
#include "shell.h"
#include "periph/spi.h"
#include "thread.h"

#include "ds3231.h"
#include "ds3231_params.h"

#include <time.h>
#include "net/sntp.h"
#include "net/ntp_packet.h"
#include "net/af.h"
#include "timex.h"



#define ISOSTR_LEN      (20U)
#define TEST_DELAY      (2U)

static ds3231_t _dev;

/* parse ISO date string (YYYY-MM-DDTHH:mm:ss) to struct tm */
static int _tm_from_str(const char *str, struct tm *time)
{
    char tmp[5];

    if (strlen(str) != ISOSTR_LEN - 1) {
        return -1;
    }
    if ((str[4] != '-') || (str[7] != '-') || (str[10] != 'T') ||
        (str[13] != ':') || (str[16] != ':')) {
        return -1;
    }

    memset(time, 0, sizeof(struct tm));

    memcpy(tmp, str, 4);
    tmp[4] = '\0';
    str += 5;
    time->tm_year = atoi(tmp) - 1900;

    memcpy(tmp, str, 2);
    tmp[2] = '\0';
    str += 3;
    time->tm_mon = atoi(tmp) - 1;

    memcpy(tmp, str, 2);
    str += 3;
    time->tm_mday = atoi(tmp);

    memcpy(tmp, str, 2);
    str += 3;
    time->tm_hour = atoi(tmp);

    memcpy(tmp, str, 2);
    str += 3;
    time->tm_min = atoi(tmp);

    memcpy(tmp, str, 2);
    time->tm_sec = atoi(tmp);

    return 0;
}

static int _cmd_get(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    char dstr[ISOSTR_LEN];

    struct tm time;
    ds3231_get_time(&_dev, &time);

    size_t pos = strftime(dstr, ISOSTR_LEN, "%Y-%m-%dT%H:%M:%S", &time);
    dstr[pos] = '\0';
    printf("The current time is: %s\n", dstr);

    return 0;
}

static int _cmd_set(int argc, char **argv)
{
    if (argc != 2) {
        printf("usage: %s <iso-date-str YYYY-MM-DDTHH:mm:ss>\n", argv[0]);
        return 1;
    }

    if (strlen(argv[1]) != (ISOSTR_LEN - 1)) {
        puts("error: input date string has invalid length");
        return 1;
    }

    struct tm target_time;
    int res = _tm_from_str(argv[1], &target_time);
    if (res != 0) {
        puts("error: unable do parse input date string");
        return 1;
    }

    ds3231_set_time(&_dev, &target_time);

    printf("success: time set to %s\n", argv[1]);
    return 0;
}


//#define IP_MAX_LEN_ADDRESS  (39)    /*IPv6 max length */
#define _DEFAULT_TIMEOUT (500000LU)
//#define _DEFAULT_TIMEOUT (5000LU)
#define HOUR_LOCAL_TIMEZONE +3

extern int sntp_sync(sock_udp_ep_t *server, uint32_t timeout);
extern int64_t sntp_get_offset(void);

static const char time_google_com[] = "216.239.35.4";
struct tm *tm;
char str_t[32];
size_t len;
char* a_adc2[]={"cmd_set",NULL};

int _ntpdate1(int argc, char **argv)
{
    uint32_t timeout = _DEFAULT_TIMEOUT;

    //if (argc < 2) {
    //    _usage1(argv[0]);
    //    return 1;
    //}
    sock_udp_ep_t server = { .port = NTP_PORT, .family = AF_INET };
    ipv4_addr_t *addr = (ipv4_addr_t *)&server.addr;

    //strncpy(_local_ip, argv[1], sizeof(_local_ip));
    //ipv4_addr_from_str(addr,argv[1]);
    ipv4_addr_from_str(addr,time_google_com);

    //if (argc > 2) {
    //    timeout = atoi(argv[2]);
    //}
    if (sntp_sync(&server, timeout) < 0) {
        puts("Error in synchronization");
        return 1;
    }
//#if defined(MODULE_NEWLIB) || defined(MODULE_PICOLIBC)
    time_t time = (time_t)(sntp_get_unix_usec() / US_PER_SEC);

    tm = gmtime(&time);
    printf("%04i-%02i-%02i %02i:%02i:%02i UTC (%i us)\n",
           tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
           tm->tm_hour + HOUR_LOCAL_TIMEZONE, tm->tm_min, tm->tm_sec,
           (int)sntp_get_offset());

           str_t[0]=0;
           len = sprintf(str_t,"%04i-%02i-%02iT%02i:%02i:%02i",
                tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
                tm->tm_hour + HOUR_LOCAL_TIMEZONE, tm->tm_min, tm->tm_sec);
           str_t[len] = 0;
           a_adc2[1]=str_t;
           _cmd_set(2, a_adc2);
           //printf("usage: %s <iso-date-str YYYY-MM-DDTHH:mm:ss>\n", argv[0]);


//#else
//    uint64_t time = sntp_get_unix_usec();
//    printf("%" PRIu32 ".%" PRIu32 " (%i us)\n",
//           (uint32_t)(time / US_PER_SEC),
//           (uint32_t)(time / US_PER_SEC),
//           (int)sntp_get_offset());
//#endif

    return 0;
}



/**
 * @brief   Default port number used for the CS pin if unassigned
 */
#ifndef DEFAULT_SPI_CS_PORT
#define DEFAULT_SPI_CS_PORT 0
#endif

/**
 * @brief   Default port number used for the CS pin if unassigned
 */
#ifndef DEFAULT_SPI_CS_PIN
#define DEFAULT_SPI_CS_PIN 0
#endif



static struct {
    spi_t dev;
    spi_mode_t mode;
    spi_clk_t clk;
    spi_cs_t cs;
} spiconf;

char stack[THREAD_STACKSIZE_MAIN];


int cmd_init(int argc, char **argv)
{
    int dev, mode, clk, port, pin, tmp;

    if (argc < 4) {
        printf("usage: %s <dev> <mode> <clk> [cs port] [cs pin]\n", argv[0]);
        puts("\tdev:");
        for (int i = 0; i < (int)SPI_NUMOF; i++) {
            printf("\t\t%i: SPI_DEV(%i)\n", i, i);
        }
        puts("\tmode:");
        puts("\t\t0: POL:0, PHASE:0 - on first rising edge");
        puts("\t\t1: POL:0, PHASE:1 - on second rising edge");
        puts("\t\t2: POL:1, PHASE:0 - on first falling edge");
        puts("\t\t3: POL:1, PHASE:1 - on second falling edge");
        puts("\tclk:");
        puts("\t\t0: 100 KHz");
        puts("\t\t1: 400 KHz");
        puts("\t\t2: 1 MHz");
        puts("\t\t3: 5 MHz");
        puts("\t\t4: 10 MHz");
        puts("\tcs port:");
        puts("\t\tPort of the CS pin, set to -1 for hardware chip select");
        puts("\tcs pin:");
        puts("\t\tPin used for chip select. If hardware chip select is enabled,\n"
             "\t\tthis value specifies the internal HWCS line");
        return 1;
    }


    /* parse the given SPI device */
    dev = atoi(argv[1]);
    if (dev < 0 || dev >= (int)SPI_NUMOF) {
        puts("error: invalid SPI device specified");
        return 1;
    }
    spiconf.dev = SPI_DEV(dev);

    /* parse the SPI mode */
    mode = atoi(argv[2]);
    switch (mode) {
        case 0: spiconf.mode = SPI_MODE_0; break;
        case 1: spiconf.mode = SPI_MODE_1; break;
        case 2: spiconf.mode = SPI_MODE_2; break;
        case 3: spiconf.mode = SPI_MODE_3; break;
        default:
            puts("error: invalid SPI mode specified");
            return 1;
    }

    /* parse the targeted clock speed */
    clk = atoi(argv[3]);
    switch (clk) {
        case 0: spiconf.clk = SPI_CLK_100KHZ; break;
        case 1: spiconf.clk = SPI_CLK_400KHZ; break;
        case 2: spiconf.clk = SPI_CLK_1MHZ;   break;
        case 3: spiconf.clk = SPI_CLK_5MHZ;   break;
        case 4: spiconf.clk = SPI_CLK_10MHZ;  break;
        default:
            puts("error: invalid bus speed specified");
            return 1;
    }

    /* parse chip select port and pin */
    if (argc > 5) {
        pin = atoi(argv[5]);
    }
    else {
        pin = DEFAULT_SPI_CS_PIN;
    }

    if (argc > 4) {
        port = atoi(argv[4]);
    }
    else {
        port = DEFAULT_SPI_CS_PORT;
    }

    if (pin < 0 || port < -1) {
        puts("error: invalid CS port/pin combination specified");
    }
    if (port == -1) {                    /* hardware chip select line */
        spiconf.cs = SPI_HWCS(pin);
    }
    else {
        spiconf.cs = (spi_cs_t)GPIO_PIN(port, pin);
    }

    /* test setup */
    tmp = spi_init_cs(spiconf.dev, spiconf.cs);
    if (tmp != SPI_OK) {
        puts("error: unable to initialize the given chip select line");
        return 1;
    }
    tmp = spi_acquire(spiconf.dev, spiconf.cs, spiconf.mode, spiconf.clk);
    if (tmp == SPI_NOMODE) {
        puts("error: given SPI mode is not supported");
        return 1;
    }
    else if (tmp == SPI_NOCLK) {
        puts("error: targeted clock speed is not supported");
        return 1;
    }
    else if (tmp != SPI_OK) {
        puts("error: unable to acquire bus with given parameters");
        return 1;
    }
    spi_release(spiconf.dev);

    printf("SPI_DEV(%i) initialized: mode: %i, clk: %i, cs_port: %i, cs_pin: %i\n",
           dev, mode, clk, port, pin);

    return 0;
}



static const unsigned char sprite8x16[10][16]=
 {
 {0b00000000
 ,0b01111000
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b01111000
 ,0b00000000
 ,0b00000000}//0
,{0b00000000
 ,0b00000100
 ,0b00010100
 ,0b01000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000000
 ,0b00000000}//1
,{0b00000000
 ,0b01111000
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b01111000
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b01111000
 ,0b00000000
 ,0b00000000}//2
,{0b00000000
 ,0b11111000
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b11111100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b11111000
 ,0b00000000
 ,0b00000000}//3
,{0b00000000
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b01111100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000000
 ,0b00000000}//4
,{0b00000000
 ,0b01111000
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b01111000
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b01111000
 ,0b00000000
 ,0b00000000}//5
,{0b00000000
 ,0b01111000
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b11111000
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b01111000
 ,0b00000000
 ,0b00000000}//6
,{0b00000000
 ,0b11111000
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000000
 ,0b00000000}//7
,{0b00000000
 ,0b01111000
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b01111000
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b01111000
 ,0b00000000
 ,0b00000000}//8
,{0b00000000
 ,0b01111000
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b01111100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b01111000
 ,0b00000000
 ,0b00000000}//9
 };


#define max7219_reg_noop        0x00
#define max7219_reg_digit0      0x01
#define max7219_reg_digit1      0x02
#define max7219_reg_digit2      0x03
#define max7219_reg_digit3      0x04
#define max7219_reg_digit4      0x05
#define max7219_reg_digit5      0x06
#define max7219_reg_digit6      0x07
#define max7219_reg_digit7      0x08
#define max7219_reg_decodeMode  0x09
#define max7219_reg_intensity   0x0a
#define max7219_reg_scanLimit   0x0b
#define max7219_reg_shutdown    0x0c
#define max7219_reg_displayTest 0x0f

unsigned char reverse_bit_in_byte(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

int num_use_module=1;
gpio_t pin15;

int setCommand(unsigned char command, unsigned char value)
{
    /* get access to the bus */
    if (spi_acquire(spiconf.dev, spiconf.cs,
                    spiconf.mode, spiconf.clk) != SPI_OK) {
        puts(" error sendCommand - spi_acquire \t\n");
        return 1;
    }
    gpio_write(pin15,0);
//    xtimer_usleep(1);

    for (int j=0; j<num_use_module; j++) {
      //printf("set Command  = %i , value= %i.\n", (int)command, (int)value);
      spi_transfer_byte(spiconf.dev, spiconf.cs, true, command);
      if(command>=max7219_reg_digit0 && command<=max7219_reg_digit7 && j>3)
       spi_transfer_byte(spiconf.dev, spiconf.cs, true, reverse_bit_in_byte(value));
       else
       spi_transfer_byte(spiconf.dev, spiconf.cs, true, value);
      //if(command>=max7219_reg_digit0 && command<=max7219_reg_digit7)buffer_matrix[j*8+command-1]=value;
      }
    spi_release(spiconf.dev);

    gpio_write(pin15,1);
    
    return 0;
}




unsigned char h_1=0;
unsigned char h_2=0;
unsigned char m_1=0;
unsigned char m_2=0;

unsigned char c_sec=0;
unsigned char b_intensity=0x01;

int setCommand2(unsigned char command)
{
    if(!(command>=max7219_reg_digit0 && command<=max7219_reg_digit7))return 1;

    /* get access to the bus */
    if (spi_acquire(spiconf.dev, spiconf.cs,
                    spiconf.mode, spiconf.clk) != SPI_OK) {
        puts(" error sendCommand - spi_acquire \t\n");
        return 1;
    }
    gpio_write(pin15,0);

    unsigned char value1=0;
    unsigned char value2=0;
    unsigned char value01=0;
    unsigned char value02=0;
    for (int j=0; j<num_use_module; j++) {
      spi_transfer_byte(spiconf.dev, spiconf.cs, true, command); // позиция

      // Декодер, что же в эту позицию поставить!
      switch(j)
	{
	case 0:
	case 7:
               {
		value1=sprite8x16[h_1][7+command]; //botton               
		value2=sprite8x16[h_1][8-command]; // top               
               };break;
	case 1:
	case 6:
               {
		value1=sprite8x16[h_2][7+command]; //botton               
		value2=sprite8x16[h_2][8-command]; // top               
               };break;
	case 3:
	case 4:
               {
		value1=sprite8x16[m_1][7+command]; //botton               
		value2=sprite8x16[m_1][8-command]; // top               
               };break;
	case 2:
	case 5:
               {
		value1=sprite8x16[m_2][7+command]; //botton               
		value2=sprite8x16[m_2][8-command]; // top               
               };break;
	}

      // отодвинуть две последнии цифры, минуты, на цеферблате
      if((j==2 || j==3 || j==4 || j==5))
             {
             value01=value1>>2;
             value02=value2>>2;
             }
          else
             {
             value01=value1;
             value02=value2;
             }

      // Делаем точечки :
      if(c_sec==1)
       {
       if((j==2 || j==5) && (command==2 || command==3))
              {
              value01=value01 | 0b10000000;
              value02=value02 | 0b10000000;
              }
       if((j==1 || j==6) && (command==2 || command==3))
              {
              value01=value01 | 0b00000001;
              value02=value02 | 0b00000001;
              }
       }

      if(b_intensity>15)
	{
              value01=~value01;
              value02=~value02;
	}

      // смотрим, нижную или верхнюю часть выводить...
      if(j>3)
       spi_transfer_byte(spiconf.dev, spiconf.cs, true, reverse_bit_in_byte(value02));
       else
       spi_transfer_byte(spiconf.dev, spiconf.cs, true, (value01));
      }
    spi_release(spiconf.dev);

    gpio_write(pin15,1);
    
    return 0;
}


void clear(void)
{
    if (spiconf.dev == SPI_UNDEF) {
    }

    for (int i = 0; i < max7219_reg_digit7; i++) {  // 8*8 dots
         setCommand(i+1, 0x0); //
      }
}

/**
 * @brief Transfer one byte on the given SPI bus
 *
 * @param[in] bus       SPI device to use
 * @param[in] cs        chip select pin/line to use, set to SPI_CS_UNDEF if chip
 *                      select should not be handled by the SPI driver
 * @param[in] cont      if true, keep device selected after transfer
 * @param[in] out       byte to send out
 *
 * @return              the received byte
 */
//uint8_t spi_transfer_byte(spi_t bus, spi_cs_t cs, bool cont, uint8_t out);

/**
 * @brief   Transfer a number bytes using the given SPI bus
 *
 * @param[in]  bus      SPI device to use
 * @param[in]  cs       chip select pin/line to use, set to SPI_CS_UNDEF if chip
 *                      select should not be handled by the SPI driver
 * @param[in]  cont     if true, keep device selected after transfer
 * @param[in]  out      buffer to send data from, set NULL if only receiving
 * @param[out] in       buffer to read into, set NULL if only sending
 * @param[in]  len      number of bytes to transfer
 */
//void spi_transfer_bytes(spi_t bus, spi_cs_t cs, bool cont,  const void *out, void *in, size_t len);



/*
The ESP hardware provides two SPI busses, with IDs 0, and 1, which map to pins generally labelled SPI and HSPI. 
If you are using any kind of development board which provides flash, then bus ID 0 (SPI) is almost certainly used for communicating with the flash chip.
You probably want to choose bus ID 1 (HSPI) for your communication, as you will have uncontended use of it.

HSPI signals are fixed to the following IO indices and GPIO pins:

Signal	IO index	ESP8266(nodemcuv3) pin
HSPI CLK	5	GPIO14         - clk 
HSPI /CS	8	GPIO15         - cs
HSPI MOSI	7	GPIO13         - din
HSPI MISO	6	GPIO12
V USB		VU			vpp
GND		gnd			gnd

nodemcuv3 -  DS3231 
G  	- 	GND
3V 	- 	VCC
D1 	- 	SCL
D2 	-	SDA

nodemcuv3 -  Botton 
GPIO2	- D4 - 
G	-    - 


*/


int refresh(void)
{
    if (spiconf.dev == SPI_UNDEF) {
        puts(" error init \t\n");
        return 1; 
    }
    setCommand(max7219_reg_scanLimit, 0x07);      
    setCommand(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
    setCommand(max7219_reg_shutdown, 0x01);    // not in shutdown mode
    setCommand(max7219_reg_displayTest, 0x00); // no display test
    setCommand(max7219_reg_intensity, b_intensity);
    //clear();
    for (int i = 0; i < max7219_reg_digit7; i++) {  // 8*8 dots
         setCommand2(i+1);  
      }
return 0;
}

int i_sec_cur=0;

void *thread_handler(void *arg)
{
    /* ... */
    (void)arg;

    while(true)
     {
     refresh();

     struct tm time;
     ds3231_get_time(&_dev, &time);

     h_1=time.tm_hour/10;
     h_2=time.tm_hour%10;

     m_2=time.tm_min/10;
     m_1=time.tm_min%10;

     if(time.tm_sec%2==0)
      c_sec=1;
      else
      c_sec=0;
     //xtimer_sleep(1);
     if(i_sec_cur!=0)
       {
       i_sec_cur--;
       h_1=b_intensity/10;
       h_2=b_intensity%10;
       }
     xtimer_usleep(100000);
     }

    return NULL;
}

/* assuming that first button is always BTN0 */
static void btn_handler(void *arg)
{
    (void) arg;
    if(i_sec_cur!=0)return;

    //printf("Button pressed!  b_intensity = %i \n", (int)b_intensity);
    if(b_intensity++==32)b_intensity=0x00;  
    i_sec_cur=3;
}


static const shell_command_t shell_commands[] = {
    //{ "init", "Setup a particular SPI configuration", cmd_init },
    { "time_get", "init as output (push-pull mode)", _cmd_get },
    { "time_set", "init as input w/o pull resistor", _cmd_set },
    { "ntpdate", "synchronizes with a remote time server", _ntpdate1 },
    { NULL, NULL, NULL }
};

int main(void)
{
    num_use_module=8;

    pin15 = GPIO_PIN(0,15);
    gpio_init(pin15,GPIO_OUT);

    xtimer_sleep(1);
    
    puts("Manual SPI peripheral driver test");
    puts("Refer to the README.md file for more information.\n");

    printf("There are %i SPI devices configured for your platform.\n",
           (int)SPI_NUMOF);


    /* reset local SPI configuration */
    spiconf.dev = SPI_UNDEF;

    if (spiconf.dev == SPI_UNDEF) {
       char* a1[]={"init","0","0","2",NULL,NULL};
       if(cmd_init(4, a1)!=0) {
        puts(" error init SPI \t\n");
        //return 1;
        }
    }


    int res;

    puts("DS3231 RTC test\n");

    /* initialize the device */
    res = ds3231_init(&_dev, &ds3231_params[0]);
    if (res != 0) {
        puts("error: unable to initialize DS3231 [I2C initialization error]");
        return 1;
    }

    /* initialize a button to manually trigger an update */
    //gpio_init_int(BTN0_PIN, BTN0_MODE, GPIO_FALLING, cb, NULL);
    gpio_init_int(GPIO_PIN(0, 2), GPIO_IN_PU, GPIO_RISING, btn_handler, NULL);
    
    thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST,
                  thread_handler, NULL,
                  "thread");
    
    /* run the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
