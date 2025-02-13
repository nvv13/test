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


int 
cmd_init(int argc, char **argv)
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



static const uint8_t sprite8x16[10][16]=
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

uint8_t reverse_bit_in_byte(uint8_t b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

int num_use_module=1;
gpio_t pin15;
gpio_t pin13;


int setCommand(uint8_t command, uint8_t value)
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




uint8_t h_1=0;
uint8_t h_2=0;
uint8_t m_1=0;
uint8_t m_2=0;

uint8_t c_sec=0;
uint8_t b_intensity=0x06;

int setCommand2(uint8_t command)
{
    if(!(command>=max7219_reg_digit0 && command<=max7219_reg_digit7))return 1;

    /* get access to the bus */
    if (spi_acquire(spiconf.dev, spiconf.cs,
                    spiconf.mode, spiconf.clk) != SPI_OK) {
        puts(" error sendCommand - spi_acquire \t\n");
        return 1;
    }
    gpio_write(pin15,0);

    uint8_t value1=0;
    uint8_t value2=0;
    uint8_t value01=0;
    uint8_t value02=0;
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
STM32

bluepill

Signal	index	bluepill    pin matrix LED
SCLK1	6	PA5         - clk 
SS1	7	PA4         - cs
MOSI1	4	PA7         - din
MISO1	5	PA6
5V      5V      5V          VCC
GND     GND     GND         GND

Signal	index	bluepill    pin button
A1      10      PA1         SWitch
GND     GND     GND         GND

Signal	index	bluepill    pin 
3V3     3V3     3V3         foto_resistor
A0(ADC) 9       PA0         resistor 47k + foto_resistor
GND     GND     GND         resistor 47k

Signal	index	bluepill    pin DS3231
SDA1    15      PB7         SDA
SCL1    16      PB6         SCL
3V3     3V3     3V3         VCC
GND     GND     GND         GND


Signal	index	bluepill    pin rs232 ttl adapter - 115200 8N1
TX2     9       PA2         RX
RX2     8       PA3         TX
GND     GND     GND         GND


*/


int refresh(void)
{
    if (spiconf.dev == SPI_UNDEF) {
        puts(" error init \t\n");
        return 1; 
    }
    setCommand(max7219_reg_scanLimit, 0x07);      
    setCommand(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
    //
    setCommand(max7219_reg_shutdown, 0x01);    // not in shutdown mode
    //
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

     if(i_sec_cur!=0)
       {
       i_sec_cur--;
       h_1=b_intensity/10;
       h_2=b_intensity%10;
       }

     xtimer_usleep(100000);

     gpio_write(pin13,c_sec);

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
    { NULL, NULL, NULL }
};

int main(void)
{

    xtimer_sleep(1);

    num_use_module=8;

    pin15 = GPIO_PIN(0,4);
    gpio_init(pin15,GPIO_OUT);

    pin13 = GPIO_PIN(PORT_C, 13);
    gpio_init(pin13,GPIO_OUT);

    
    puts("Manual SPI peripheral driver test");
    puts("Refer to the README.md file for more information.\n");

    printf("There are %i SPI devices configured for your platform.\n",
           (int)SPI_NUMOF);


    /* reset local SPI configuration */
    spiconf.dev = SPI_UNDEF;

    if (spiconf.dev == SPI_UNDEF) {
       char* a1[]={"init","0","0","1",NULL,NULL};
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
        while(true)
         {
         gpio_write(pin13,1);
         xtimer_usleep(30000);
         gpio_write(pin13,0);
         xtimer_usleep(50000);
         }
        return 1;
    }

    /* initialize a button to manually trigger an update */
    //gpio_init_int(BTN0_PIN, BTN0_MODE, GPIO_FALLING, cb, NULL);
    gpio_init_int(GPIO_PIN(0, 1), GPIO_IN_PU, GPIO_RISING, btn_handler, NULL);
    
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
