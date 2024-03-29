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

#include "xtimer.h"
#include "shell.h"
#include "periph/spi.h"
#include "thread.h"

//#include "periph/gpio.h"


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

#ifdef MODULE_PERIPH_SPI_RECONFIGURE
int cmd_spi_gpio(int argc, char **argv)
{
    int dev = -1;

    /* parse the given SPI device */
    if (argc > 1) {
        dev = atoi(argv[1]);
    }

    if (dev < 0 || dev >= (int)SPI_NUMOF) {
        puts("error: invalid SPI device specified");
        return 1;
    }

    gpio_t miso_pin = spi_pin_miso(dev);
    gpio_t mosi_pin = spi_pin_mosi(dev);

    printf("Command: spi_deinit_pins(%i)\n", dev);
    spi_deinit_pins(dev);

    gpio_init(miso_pin, GPIO_OUT);
    gpio_init(mosi_pin, GPIO_OUT);

    xtimer_sleep(1);

    printf("Command: gpio_set()\n");
    gpio_set(miso_pin);
    gpio_set(mosi_pin);

    xtimer_sleep(1);

    printf("Command: gpio_clear()\n");
    gpio_clear(miso_pin);
    gpio_clear(mosi_pin);

    xtimer_sleep(1);

    printf("Command: spi_init_pins(%i)\n", dev);
    spi_init_pins(dev);

    printf("Success: spi_%i re-init\n", dev);
    return 0;
}
#endif


unsigned char buffer_matrix[80];
int num_use_module=1;

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

int setCommand2(unsigned char command, unsigned char value1, unsigned char value2)
{
    /* get access to the bus */
    if (spi_acquire(spiconf.dev, spiconf.cs,
                    spiconf.mode, spiconf.clk) != SPI_OK) {
        puts(" error sendCommand - spi_acquire \t\n");
        return 1;
    }
    gpio_write(pin15,0);
//    xtimer_usleep(1);

    unsigned char value01;
    unsigned char value02;
    for (int j=0; j<num_use_module; j++) {
      //printf("set Command  = %i , value= %i.\n", (int)command(int)value);
      spi_transfer_byte(spiconf.dev, spiconf.cs, true, command);

      if(command>=max7219_reg_digit0 && command<=max7219_reg_digit7 && (j==2 || j==3 || j==4 || j==5))
             {
             value01=value1>>2;
             value02=value2>>2;
             }
          else
             {
             value01=value1;
             value02=value2;
             }

      if(command>=max7219_reg_digit0 && command<=max7219_reg_digit7 && j>3)
       spi_transfer_byte(spiconf.dev, spiconf.cs, true, reverse_bit_in_byte(value02));
       else
       spi_transfer_byte(spiconf.dev, spiconf.cs, true, (value01));
      //if(command>=max7219_reg_digit0 && command<=max7219_reg_digit7)buffer_matrix[j*8+command-1]=value;
      }
    spi_release(spiconf.dev);

    gpio_write(pin15,1);
    
    return 0;
}


void reload(void)
{
    if (spi_acquire(spiconf.dev, spiconf.cs,
                    spiconf.mode, spiconf.clk) != SPI_OK) {
            return;
         }
    gpio_write(pin15,0);
         
    for (int i=0; i<max7219_reg_digit7; i++)
    {
	int command = i+1;
	for (int j=0; j<num_use_module; j++) {
            printf("set Command  = %i , value= %i.\n", (int)command, (int)buffer_matrix[j*8+command-1]);
            spi_transfer_byte(spiconf.dev, spiconf.cs, true, command); // 1-8
            spi_transfer_byte(spiconf.dev, spiconf.cs, true, buffer_matrix[j*8+command-1]); 
	 }
    }
    
    spi_release(spiconf.dev);
    gpio_write(pin15,1);
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

Signal	IO index	ESP8266 pin
HSPI CLK	5	GPIO14         - clk 
HSPI /CS	8	GPIO15         - cs
HSPI MOSI	7	GPIO13         - din
HSPI MISO	6	GPIO12

V USB		VU			vpp
GND		gnd			gnd

*/
int cmd_send_buf(int argc, char **argv)
{
    if (spiconf.dev == SPI_UNDEF) {
        puts(" error init \t\n");
        return 1; 
    }

    setCommand(max7219_reg_scanLimit, 0x07);      
    setCommand(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
    setCommand(max7219_reg_shutdown, 0x01);    // not in shutdown mode
    setCommand(max7219_reg_displayTest, 0x00); // no display test
    setCommand(max7219_reg_intensity, 0x08);
    clear();




    for (int i = 0; i < max7219_reg_digit7; i++) {  // 8*8 dots
         setCommand(i+1, 0xAA); // 10101010 
      }

    xtimer_sleep(1);

    for (int i = 0; i < max7219_reg_digit7; i++) {  // 8*8 dots
         setCommand(i+1, reverse_bit_in_byte(0xAA)); // 10101010 
      }

    xtimer_sleep(1);

    clear();
    
    puts(" -> Test1 \t\n");
    return 0;
}

int cmd2_send_buf(int argc, char **argv)
{

    if (spiconf.dev == SPI_UNDEF) {
        puts(" error init \t\n");
        return 1; 
    }

    setCommand(max7219_reg_scanLimit, 0x07);      
    setCommand(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
    setCommand(max7219_reg_shutdown, 0x01);    // not in shutdown mode
    setCommand(max7219_reg_displayTest, 0x00); // no display test
    setCommand(max7219_reg_intensity, 0x0f);
    clear();

    puts(" -> clear \t\n");

    return 0;
}


int cmd3_send_buf(int argc, char **argv)
{
    if (spiconf.dev == SPI_UNDEF) {
        puts(" error init \t\n");
        return 1; 
    }

    setCommand(max7219_reg_scanLimit, 0x07);      
    setCommand(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
    setCommand(max7219_reg_shutdown, 0x01);    // not in shutdown mode
    setCommand(max7219_reg_displayTest, 0x00); // no display test
    setCommand(max7219_reg_intensity, 0x0f);
    clear();

    for (int i = 0; i < max7219_reg_digit7; i++) {  // 8*8 dots
         setCommand(i+1, 0xff); //
      }

    puts(" -> fill \t\n");

    return 0;
}
/*
00000000 00000000 00000000 00000000 
00000000 00000000 00000000 00000000 
00000000 00000000 00000000 00000000 
00000000 00000000 00000000 00000000 
00000000 00000000 00000000 00000000 
00000000 00000000 00000000 00000000 
00000000 00000000 00000000 00000000 
00000000 00000000 00000000 00000000 

00000000 00000000 00000000 00000000 
00000000 00000000 00000000 00000000 
00000000 00000000 00000000 00000000 
00000000 00000000 00000000 00000000 
00000000 00000000 00000000 00000000 
00000000 00000000 00000000 00000000 
00000000 00000000 00000000 00000000 
00000000 00000000 00000000 00000000 

-----------------------------------
00000000 00000000 00000000 00000000 
01111100 00000000 00000000 00000000 
10000010 00000000 00000000 00000000 
10000010 00000000 00000000 00000000 
10000010 00000000 00000000 00000000 
10000010 00000000 00000000 00000000 
10000010 00000000 00000000 00000000 
10000010 00000000 00000000 00000000 

10000010 00000000 00000000 00000000 
10000010 00000000 00000000 00000000 
10000010 00000000 00000000 00000000 
10000010 00000000 00000000 00000000 
10000010 00000000 00000000 00000000 
10000010 00000000 00000000 00000000 
01111100 00000000 00000000 00000000 
00000000 00000000 00000000 00000000 

-----------------------------------
00000000 00000000 00000000 00000000 
01111000 11110000 00001111 00011110 
10000101 00001000 00010000 10100001 
10000101 00001000 00010000 10100001 
10000101 00001000 00010000 10100001 
10000101 00001001 10010000 10100001 
10000101 00001000 00010000 10100001 
10000101 00001000 00010000 10100001 

10000101 00001000 00010000 10100001 
10000101 00001000 00010000 10100001 
10000101 00001001 10010000 10100001 
10000101 00001000 00010000 10100001 
10000101 00001000 00010000 10100001 
10000101 00001000 00010000 10100001 
01111000 11110000 00001111 00011110 
00000000 00000000 00000000 00000000 

field 32*16

bytes - gorizontal

*/

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
 ,0b00000000}//1
,{0b00000000
 ,0b11111100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b11111100
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b11111100
 ,0b00000000
 ,0b00000000}//2
,{0b00000000
 ,0b11111100
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
 ,0b11111100
 ,0b00000000
 ,0b00000000}//3
,{0b00000000
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b11111100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000000
 ,0b00000000}//4
,{0b00000000
 ,0b11111100
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b11111100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b11111100
 ,0b00000000
 ,0b00000000}//5
,{0b00000000
 ,0b11111100
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b10000000
 ,0b11111100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b11111100
 ,0b00000000
 ,0b00000000}//6
,{0b00000000
 ,0b11111100
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
 ,0b11111100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b11111100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b11111100
 ,0b00000000
 ,0b00000000}//8
,{0b00000000
 ,0b11111100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b10000100
 ,0b11111100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b00000100
 ,0b11111100
 ,0b00000000
 ,0b00000000}//9
 };

void copy_sprite8x16_to_video_buf(const unsigned char* in)
{
for(int i=0;i<16;i++)
 {
 buffer_matrix[i]=in[i]; 
 }
return;
}

unsigned char ind=0;

void *thread_handler(void *arg)
{
    /* ... */
    (void)arg;

//    char* a0[]={"temp"};

    while(true)
     {
     //gpio_toggle(pin);
     //xtimer_sleep(1);

//     _cmd_readtemp(1, a0);
     //clear();
     //copy_sprite8x16_to_video_buf(sprite8x16[ind]);
     //reload();

     //for (int i = 0; i < max7219_reg_digit7; i++) {  // 8*8 dots
     //    setCommand(i+1, ind); // 10101010 
     // }



    if (spiconf.dev == SPI_UNDEF) {
        puts(" error init \t\n");
        return NULL; 
    }

    setCommand(max7219_reg_scanLimit, 0x07);      
    setCommand(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
    setCommand(max7219_reg_shutdown, 0x01);    // not in shutdown mode
    setCommand(max7219_reg_displayTest, 0x00); // no display test
    setCommand(max7219_reg_intensity, 0x08);
    clear();

    //xtimer_usleep(10);




    for (int i = 0; i < max7219_reg_digit7; i++) {  // 8*8 dots
         //setCommand(i+1, ind); 
         //setCommand2(i+1, sprite8x16[ind][7-i] //botton
         //                ,sprite8x16[ind][8+i] // top
         //                );  
         setCommand2(i+1, sprite8x16[ind][8+i] //botton
                         ,sprite8x16[ind][7-i] // top
                         );  
      }


     if(ind++ == 9)
      {
      ind=0;
      }

     xtimer_sleep(1);
     //xtimer_usleep(1000);
     }

    return NULL;
}


static const shell_command_t shell_commands[] = {
    { "init", "Setup a particular SPI configuration", cmd_init },
#ifdef MODULE_PERIPH_SPI_RECONFIGURE
    { "spi_gpio", "Re-configures MISO & MOSI pins to GPIO mode and back.", cmd_spi_gpio },
#endif
    { "test1", "send test1 8 to 8*8 ", cmd_send_buf },
    { "clear", "clear 8x8", cmd2_send_buf },
    { "fill", "fill 8x8", cmd3_send_buf },
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
