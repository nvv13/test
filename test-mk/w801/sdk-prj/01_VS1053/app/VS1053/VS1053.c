/**
 * This is a driver library for VS1053 MP3 Codec Breakout
 * (Ogg Vorbis / MP3 / AAC / WMA / FLAC / MIDI Audio Codec Chip).
 * Adapted for Espressif ESP8266 and ESP32 boards.
 *
 * version 1.0.1
 *
 * Licensed under GNU GPLv3 <http://gplv3.fsf.org/>
 * Copyright © 2018
 *
 * @authors baldram, edzelf, MagicCube, maniacbug
 *
 * Development log:
 *  - 2011: initial VS1053 Arduino library
 *          originally written by J. Coliz (github: @maniacbug),
 *  - 2016: refactored and integrated into Esp-radio sketch
 *          by Ed Smallenburg (github: @edzelf)
 *  - 2017: refactored to use as PlatformIO library
 *          by Marcin Szalomski (github: @baldram | twitter: @baldram)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License or later
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "wm_type_def.h"
//#include "wm_uart.h"
#include "wm_gpio.h"
#include "wm_gpio_afsel.h"
#include "wm_hostspi.h"
//#include "wm_socket.h"
//#include "wm_sockets.h"
//#include "wm_wifi.h"
//#include "wm_hspi.h"
//#include "wm_pwm.h"
//#include "wm_params.h"
#include "wm_osal.h"
//#include "wm_netif.h"
//#include "wm_efuse.h"
//#include "wm_mem.h"
//#include "wm_cpu.h"
//#include "wm_regs.h"
//#include "wm_rtc.h"
//#include "wm_timer.h"
//#include "wm_watchdog.h"
//#include "csi_core.h"

#include "HTTPClient.h"
#include "wm_type_def.h"
#include "wm_watchdog.h"

#include "patches/vs1053b-patches.h"

#include "ConsoleLogger.h"

#include "VS1053.h"

#define FCLK_SLOW_VS1053 200000
#define FCLK_FAST_VS1053 4000000
#define FCLK_SUPER_FAST_VS1053 6000000
//-----------------------------------------------------
#ifndef _BV
#define _BV(x) (1UL << (x))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
//-----------------------------------------------------
static long
map (long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//-----------------------------------------------------
static void
delay (u32 ms)
{
  u32 tick = ms / (1000 / HZ);
  if (ms > (tick * (1000 / HZ)))
    tick++; //в данном случае в большую сторону сделаем
  tls_os_time_delay (tick);
};

  //-----------------------------------------------------

#define HIGH 1
#define LOW 0

#define INPUT_PULLUP WM_GPIO_ATTR_PULLHIGH
#define INPUT WM_GPIO_DIR_INPUT
#define OUTPUT WM_GPIO_DIR_OUTPUT

static u8
digitalRead (enum tls_io_name pin)
{
  return tls_gpio_read (pin);
};
static void
digitalWrite (enum tls_io_name pin, u8 val)
{
  tls_gpio_write (pin, val);
};
//-----------------------------------------------------
static void
yield (void)
{
  tls_os_time_delay (1);
}; // Very short delay

//-----------------------------------------------------
#define SPISettings int
// SPISettings(200000, MSBFIRST, SPI_MODE0);
//#define MSBFIRST 0
//#define SPI_MODE0 0

static enum tls_io_name
    spi_cs; /* даже если не используеться, надо определить?  */
static enum tls_io_name spi_ck; /* */
static enum tls_io_name spi_di; /* */
static enum tls_io_name spi_do; /* */
static u8
    spi_fastest_speed; // 0 - 4 MHz работает на большенстве плат, 1 - 6 MHz это
                       // может приводить к срыву передачи на некоторых платах

static void
SPI_print_retval (int i_retval, char *help)
{
  switch (i_retval)
    {
    case TLS_SPI_STATUS_OK:
      // LOG (" %s = TLS_SPI_STATUS_OK success.\n", help);
      break;
    case TLS_SPI_STATUS_EINVAL:
      LOG (" %s = TLS_SPI_STATUS_EINVAL argument is invalid.\n", help);
      break;
    case TLS_SPI_STATUS_ENOMEM:
      LOG (" %s = TLS_SPI_STATUS_ENOMEM no enough memory.\n", help);
      break;
    case TLS_SPI_STATUS_ESHUTDOWN:
      LOG (" %s = TLS_SPI_STATUS_ESHUTDOWN SPI driver has not been "
           "installed.\n",
           help);
      break;
    case TLS_SPI_STATUS_EMODENOSUPPORT:
      LOG (" %s = TLS_SPI_STATUS_EMODENOSUPPORT mode is not support.\n", help);
      break;
    case TLS_SPI_STATUS_ECLKNOSUPPORT:
      LOG (" %s = TLS_SPI_STATUS_ECLKNOSUPPORT	fclk is not support.\n", help);
      break;
    case TLS_SPI_STATUS_EBUSY:
      LOG (" %s = TLS_SPI_STATUS_EBUSY	SPI is already initialized.\n", help);
      break;
    }
}

static int
SPI_Settings (u32 fclk)
{
  wm_spi_cs_config (spi_cs);
  wm_spi_ck_config (spi_ck);
  wm_spi_di_config (spi_di);
  wm_spi_do_config (spi_do);
  // tls_spi_trans_type (SPI_BYTE_TRANSFER);
  tls_spi_trans_type (SPI_DMA_TRANSFER);
  // tls_spi_trans_type (SPI_WORD_TRANSFER);
  // SPI_DMA_TRANSFER);
  // SPI_WORD_TRANSFER=spi_set_endian(0)=SPI_LITTLE_ENDIAN;
  // default SPI_BYTE_TRANSFER=spi_set_endian(1)=SPI_BIG_ENDIAN;
  // byte,word, dma
  // используемая последовательность вывода бит.
  // MSBFIRST (Most Significant Bit First) — слева - с первого (левого) бита
  // (старшего) или LSBFIRST (Least Significant Bit First) — справа - с
  // последнего бита (младшего)
  //  надо чтобы было  MSBFIRST, SPI_MODE0
  int retval = tls_spi_setup (TLS_SPI_MODE_0, TLS_SPI_CS_LOW, fclk);
  /**< SPI transfer mode: mode_0(CPHA=0, CHOL=0),
         mode_1(CPHA=0, CHOL=1), mode_2(CPHA=1,
             CHOL=0), mode_3(CPHA=1, CHOL=1). */
  SPI_print_retval (retval, "tls_spi_setup");
  return retval;
};
static void SPI_beginTransaction (int VS1053_SPI){}; // Prevent other SPI users
static void SPI_endTransaction (void){};             // Allow other SPI users

/*
static int
SPI_transfer (const u8 *txbuf, u32 n_tx, u8 *rxbuf, u32 n_rx)
{
  // @brief          This function is used to synchronously write command and
  // then read data from SPI.
  // int tls_spi_read_with_cmd(const u8 * txbuf, u32 n_tx, u8 * rxbuf, u32
  // n_rx);
  int retval = tls_spi_read_with_cmd (txbuf, n_tx, rxbuf, n_rx);
  SPI_print_retval (retval, "SPI_transfer");
  return retval;
};
*/

static int
SPI_read_buf (u8 *rxbuf, u32 n_rx)
{
  int retval = tls_spi_read (rxbuf, n_rx);
  SPI_print_retval (retval, "SPI_read");
  return retval;
};

/*
static u16
SPI_read16 (u16 a)
{
  // u8 txbuf[2] = { a };
  u8 rxbuf[2] = { 0, 0 };
  SPI_print_retval (tls_spi_read (rxbuf, 2), "SPI_read16");
  //  @brief          This function is used to synchronously write command and
  //  then read data from SPI.
  // int tls_spi_read_with_cmd(const u8 * txbuf, u32 n_tx, u8 * rxbuf, u32
  // n_rx);
  // SPI_print_retval (tls_spi_read_with_cmd (txbuf, 1, rxbuf, 1),
  //                  "SPI_transfer");
  return rxbuf[0] << 8 | rxbuf[1];
};*/
static void
SPI_write (u8 a)
{
  u8 buf[1];
  buf[0] = a;
  SPI_print_retval (tls_spi_write (buf, 1), "SPI_write");
};
/*static void
SPI_write16 (u16 a)
{
  u8 buf[2];
  union
  {
    u16 w;
    u8 b[2];
  } un;
  un.w = a;
  // CPU:xt804 - little-endiann
  buf[0] = un.b[1]; // first Send hi byte
  buf[1] = un.b[0]; // second Send lo byte
  SPI_print_retval (tls_spi_write (buf, 2), "SPI_write16");
};*/ // Send 16 bits data
static void
SPI_writeBytes (u8 *data, size_t chunk_length)
{
  SPI_print_retval (tls_spi_write (data, chunk_length), "SPI_writeBytes");
};

//-----------------------------------------------------

/* The following macro is for VS1063, VS1053, VS1033, VS1003, VS1103.
   Divide hz by two when calling if SM_CLK_RANGE = 1
 source http://www.vlsi.fi/en/support/software/microcontrollersoftware.html
*/
#define HZ_TO_SC_FREQ(hz) (((hz)-8000000 + 2000) / 4000)

/* Following are for VS1053 and VS1063 */
#define SC_MULT_53_10X 0x0000
#define SC_MULT_53_20X 0x2000
#define SC_MULT_53_25X 0x4000
#define SC_MULT_53_30X 0x6000
#define SC_MULT_53_35X 0x8000
#define SC_MULT_53_40X 0xa000
#define SC_MULT_53_45X 0xc000
#define SC_MULT_53_50X 0xe000

/* Following are for VS1053 and VS1063 */
#define SC_ADD_53_00X 0x0000
#define SC_ADD_53_10X 0x0800
#define SC_ADD_53_15X 0x1000
#define SC_ADD_53_20X 0x1800

const uint8_t VS1053_SCI_READ = 0x03;  // Serial read address
const uint8_t VS1053_SCI_WRITE = 0x02; // Serial write address

// const uint8_t vs1053_chunk_size = 32;

//* SCI Register
const uint8_t SCI_MODE = 0x00;   // Mode control
const uint8_t SCI_STATUS = 0x01; // Status of VS1053b
const uint8_t SCI_BASS = 0x02;   // Built-in bass/treble control
const uint8_t SCI_CLOCKF = 0x03; // Clock frequency + multiplier
const uint8_t SCI_DECODE_TIME
    = 0x04; //!< Decode time in seconds // current decoded time in full seconds
const uint8_t SCI_AUDATA = 0x05;   // Misc. audio data
const uint8_t SCI_WRAM = 0x06;     // RAM write/read
const uint8_t SCI_WRAMADDR = 0x07; // Base address for RAM write/read
const uint8_t SCI_HDAT0 = 0x08;    // Stream header data 0
const uint8_t SCI_HDAT1 = 0x09;    // Stream header data 1
const uint8_t SCI_AIADDR
    = 0x0A; //!< Indicates the start address of the application code written
            //!< earlier
            //!<    with SCI_WRAMADDR and SCI_WRAM registers.
const uint8_t SCI_VOL = 0x0B;     // Volume control
const uint8_t SCI_AICTRL0 = 0x0C; // SCI_AICTRL register 0. Used to access
                                  // the user's application program
const uint8_t SCI_AICTRL1 = 0x0D; // SCI_AICTRL register 1. Used to access
                                  // the user's application program
const uint8_t SCI_AICTRL2 = 0x0E; // SCI_AICTRL register 2. Used to access
                                  // the user's application program
const uint8_t SCI_AICTRL3 = 0x0F; // SCI_AICTRL register 3. Used to access
                                  // the user's application program
const uint8_t SCI_num_registers = 0x0F; //
//* end SCI Register

//* SCI_MODE bits
const uint8_t SM_DIFF
    = 0; //!< Differential, 0: normal in-phase audio, 1: left channel inverted
const uint8_t SM_LAYER12 = 1;  // Allow MPEG layers I & II, 0:no 1:yes
const uint8_t SM_RESET = 2;    // Bitnumber in SCI_MODE soft reset, 1:reset
const uint8_t SM_CANCEL = 3;   // Cancel decoding current file, 1:cancel
const uint8_t SM_EARSPKLO = 4; // Bitnumber EarSpeaker low setting, 1:active
const uint8_t SM_TESTS = 5;    // Allow SDI tests, 1:allowed
const uint8_t SM_STREAM = 6;   // Stream mode, 1:yes
const uint8_t SM_EARSPEAKER_HI = 7; // EarSpeaker high setting 0 off 1 active
const uint8_t SM_DACT = 8;          // DCLK active edge 0 rising 1 falling
const uint8_t SM_SDIORD = 9;        // SDI bit order 0 MSb first 1 MSb last
const uint8_t SM_SDISHARE = 10;     // Share SPI chip select 0 no 1 yes
const uint8_t SM_SDINEW = 11;       // Bitnumber in SCI_MODE always on
const uint8_t SM_ADPCM = 12;        // Bitnumber PCM/ADPCM recording active
// 13 bit alwaus 0, reserv
const uint8_t SM_LINE1 = 14; // MIC/LINE1 selector input, 0:MICP 1:LINE1
const uint8_t SM_CLKRANGE
    = 15; // Input clock range, 0:12..13 MHz, 1:24..26 MHz
//* end SCI_MODE bits

//* -
const uint16_t ADDR_REG_DREQ_R
    = 0xc012; // address of DREQ register, VS1053b DREQ also goes down while an
              // SCI operation is in progress.
const uint16_t ADDR_REG_GPIO_DDR_RW = 0xc017;   // Direction
const uint16_t ADDR_REG_GPIO_VAL_R = 0xc018;    // Values read from pins
const uint16_t ADDR_REG_GPIO_ODATA_RW = 0xc019; // Values set to the pin
const uint16_t ADDR_REG_INT_ENABLE = 0xC01A;    // Interrupt enable
const uint16_t ADDR_REG_I2S_CONFIG_RW = 0xc040;
//* end -

static SPISettings VS1053_SPI; // SPI settings for this slave
static uint8_t endFillByte;    // Byte to send when stopping song

static enum tls_io_name rst_pin;  // hardware Reset Pin
static enum tls_io_name cs_pin;   // Pin where CS line is connected
static enum tls_io_name dcs_pin;  // Pin where DCS line is connected
static enum tls_io_name dreq_pin; // Pin where DREQ line is connected
static uint8_t curvol = 0x40;     // Current volume setting 0..100%
static s8_t curbalance = 0;       // Current balance setting -100..100
static enum VS1053_I2S_RATE i2sRateOut = VS1053_I2S_NONE;

static void
VS1053_await_data_request (void)
{
  while (!digitalRead (dreq_pin))
    {
      yield (); // Very short delay
    }
}

static void
VS1053_control_mode_on (void)
{
  SPI_beginTransaction (VS1053_SPI); // Prevent other SPI users
  digitalWrite (dcs_pin, HIGH);      // Bring slave in control mode
  digitalWrite (cs_pin, LOW);
}

static void
VS1053_control_mode_off (void)
{
  digitalWrite (cs_pin, HIGH); // End control mode
  SPI_endTransaction ();       // Allow other SPI users
}

static void
VS1053_data_mode_on (void)
{
  SPI_beginTransaction (VS1053_SPI); // Prevent other SPI users
  digitalWrite (cs_pin, HIGH);       // Bring slave in data mode
  digitalWrite (dcs_pin, LOW);
}

static void
VS1053_data_mode_off (void)
{
  digitalWrite (dcs_pin, HIGH); // End data mode
  SPI_endTransaction ();        // Allow other SPI users
}

// static uint16_t VS1053_read_register (uint8_t _reg);

// static void VS1053_sdi_send_buffer (uint8_t *data, size_t len);

// static void VS1053_sdi_send_fillers (size_t length);

// static void VS1053_wram_write (uint16_t address, uint16_t data);

// static uint16_t VS1053_wram_read (uint16_t address);

//    inline bool VS1053_data_request(void)  {
//        return (digitalRead(dreq_pin) == HIGH);
//    }
// bool VS1053_data_request(void);
// static bool VS1053_data_request(void)
//{
//  return (digitalRead(dreq_pin) == HIGH);
//}

extern void VS1053_PlayHttpMp3_set (libVS1053_t *set_pin);

void
VS1053_VS1053 (libVS1053_t *set_pin)
{
  rst_pin = set_pin->rst_pin;
  cs_pin = set_pin->cs_pin;
  dcs_pin = set_pin->dcs_pin;
  dreq_pin = set_pin->dreq_pin;

  spi_cs = set_pin->spi_cs;
  spi_ck = set_pin->spi_ck;
  spi_di = set_pin->spi_di;
  spi_do = set_pin->spi_do;
  spi_fastest_speed = set_pin->spi_fastest_speed;
  i2sRateOut = set_pin->i2sRateOut;

  VS1053_PlayHttpMp3_set (set_pin);
}

static uint16_t
VS1053_read_register (uint8_t _reg)
{
  uint16_t result;
  VS1053_await_data_request (); // Wait for DREQ to be HIGH again
  VS1053_control_mode_on ();
  uint8_t buffer[2] = { VS1053_SCI_READ, _reg };

  // SPI_transfer (buffer, 2, buffer, 2); то-же работает

  SPI_writeBytes (buffer, 2);
  SPI_read_buf (buffer, 2); //

  result = ((uint16_t) (buffer[0]) << 8) | (uint16_t) (buffer[1]);
  VS1053_await_data_request (); // Wait for DREQ to be HIGH again
  VS1053_control_mode_off ();
  return result;
}

static void
VS1053_writeRegister (uint8_t _reg, uint16_t _value)
{
  VS1053_await_data_request (); // Wait for DREQ to be HIGH again
  VS1053_control_mode_on ();
  uint8_t buffer[4] = { VS1053_SCI_WRITE, _reg, (uint8_t) (_value >> 8),
                        (uint8_t) (_value & 0xFF) };
  SPI_writeBytes (buffer, 4);
  VS1053_await_data_request ();
  VS1053_control_mode_off ();
}

void
VS1053_softReset ()
{
  LOG ("Performing soft-reset\n");
  // Init SPI in slow mode ( 0.2 MHz )
  VS1053_SPI = SPI_Settings (FCLK_SLOW_VS1053);
  VS1053_writeRegister (SCI_MODE, _BV (SM_SDINEW) | _BV (SM_RESET));
  /* Newmode, Reset, No L1-2 */

  delay (10);
  VS1053_await_data_request ();

  /* A quick sanity check: write to two registers, then test if we
   get the same results. Note that if you use a too high SPI
   speed, the MSB is the most likely to fail when read again. */
  VS1053_writeRegister (SCI_HDAT0, 0xABAD);
  VS1053_writeRegister (SCI_HDAT1, 0x1DEA);
  if (VS1053_read_register (SCI_HDAT0) != 0xABAD
      || VS1053_read_register (SCI_HDAT1) != 0x1DEA)
    {
      printf ("There is something wrong with VS10xx\n");
    }

  // VS1053_writeRegister(SPI_CLOCKF,0XC000);   //Set the clock
  VS1053_writeRegister (SCI_AUDATA, 0xbb81); // samplerate 48k,stereo
  VS1053_writeRegister (SCI_BASS, 0x0055);   // set accent
  // VS1053_writeRegister (SCI_VOL, 0x4040);    // Set volume level
  VS1053_setVolume (curvol); // restore volume

  if (spi_fastest_speed)
    {
      VS1053_writeRegister (SCI_CLOCKF, HZ_TO_SC_FREQ (12288000)
                                            | SC_MULT_53_50X | SC_ADD_53_10X);
      // XTALI×5.0 clock settings multiplyer 5.0 = 20 MHz
      // SPI Clock to 6 MHz. Now you can set high speed SPI clock.
    }
  else
    {
      VS1053_writeRegister (SCI_CLOCKF, HZ_TO_SC_FREQ (12288000)
                                            | SC_MULT_53_30X | SC_ADD_53_10X);
      // Normal clock settings multiplyer 3.0 = 12.2 MHz
    }

  VS1053_await_data_request ();

  if (spi_fastest_speed)
    {
      VS1053_SPI = SPI_Settings (FCLK_SUPER_FAST_VS1053);
    }
  else
    {
      VS1053_SPI
          = SPI_Settings (FCLK_FAST_VS1053); // SPI Clock to 4 MHz. Now you can
                                             // set high speed SPI clock.
    }
  VS1053_enableI2sOut (i2sRateOut);
}

void
VS1053_reset (void)
{
  // TODO:
  // http://www.vlsi.fi/player_vs1011_1002_1003/modularplayer/vs10xx_8c.html#a3
  // hardware reset
  if (gpio_is_valid (rst_pin))
    {
      digitalWrite (rst_pin, LOW);
      delay (100);
      digitalWrite (rst_pin, HIGH);
      VS1053_await_data_request ();
      // DREQ rises when initialization is complete. You should not send any
      // data or commands before that.
    }

  delay (100);
  VS1053_softReset ();
  delay (100);
  VS1053_await_data_request ();
}

static void
VS1053_sdi_send_buffer (uint8_t *data, size_t len)
{
  size_t chunk_length; // Length of chunk 32 byte or shorter

  VS1053_data_mode_on ();
  while (len) // More to do?
    {
      VS1053_await_data_request (); // Wait for space available
      chunk_length = len;
      if (len > vs1053_chunk_size)
        {
          chunk_length = vs1053_chunk_size;
        }
      len -= chunk_length;
      SPI_writeBytes (data, chunk_length);
      data += chunk_length;
    }
  VS1053_data_mode_off ();
}

static void
VS1053_sdi_send_fillers (size_t len)
{
  size_t chunk_length; // Length of chunk 32 byte or shorter

  VS1053_data_mode_on ();
  while (len) // More to do?
    {
      VS1053_await_data_request (); // Wait for space available
      chunk_length = len;
      if (len > vs1053_chunk_size)
        {
          chunk_length = vs1053_chunk_size;
        }
      len -= chunk_length;
      while (chunk_length--)
        {
          SPI_write (endFillByte);
        }
    }
  VS1053_data_mode_off ();
}

static void
VS1053_wram_write (uint16_t address, uint16_t data)
{
  VS1053_writeRegister (SCI_WRAMADDR, address);
  VS1053_writeRegister (SCI_WRAM, data);
}

static uint16_t
VS1053_wram_read (uint16_t address)
{
  VS1053_writeRegister (SCI_WRAMADDR, address); // Start reading from WRAM
  return VS1053_read_register (SCI_WRAM);       // Read back result
}

bool
VS1053_testComm (const char *header)
{
  // Test the communication with the VS1053 module.  The result wille be
  // returned. If DREQ is low, there is problably no VS1053 connected.  Pull
  // the line HIGH in order to prevent an endless loop waiting for this signal.
  // The rest of the software will still work, but readbacks from VS1053 will
  // fail.
  int i; // Loop control
  uint16_t r1, r2, cnt = 0;
  uint16_t delta = 300; // 3 for fast SPI

  if (!digitalRead (dreq_pin))
    {
      LOG ("VS1053 not properly installed!\n");
      // Allow testing without the VS1053 module
      tls_gpio_cfg (dreq_pin, WM_GPIO_DIR_INPUT,
                    WM_GPIO_ATTR_PULLHIGH); // DREQ is now input with pull-up
      return false;                         // Return bad result
    }
  // Further TESTING.  Check if SCI bus can write and read without errors.
  // We will use the volume setting for this.
  // Will give warnings on serial output if DEBUG is active.
  // A maximum of 20 errors will be reported.
  if (strstr (header, "Fast"))
    {
      delta = 3; // Fast SPI, more loops
    }

  LOG ("%s", header); // Show a header

  for (i = 0; (i < 0xFFFF) && (cnt < 20); i += delta)
    {
      VS1053_writeRegister (SCI_VOL, i);   // Write data to SCI_VOL
      r1 = VS1053_read_register (SCI_VOL); // Read back for the first time
      r2 = VS1053_read_register (SCI_VOL); // Read back a second time
      if (r1 != r2 || i != r1 || i != r2)  // Check for 2 equal reads
        {
          LOG ("VS1053 error retry SB:%04X R1:%04X R2:%04X\n", i, r1, r2);
          cnt++;
          delay (10);
        }
      yield (); // Allow ESP firmware to do some bookkeeping
    }
  return (cnt == 0); // Return the result
}

void
VS1053_begin ()
{
  tls_gpio_cfg (dreq_pin, WM_GPIO_DIR_INPUT,
                WM_GPIO_ATTR_FLOATING); // DREQ is an input
  tls_gpio_cfg (cs_pin, WM_GPIO_DIR_OUTPUT,
                WM_GPIO_ATTR_FLOATING); // The SCI and SDI signals
  if (gpio_is_valid (rst_pin))
    tls_gpio_cfg (rst_pin, WM_GPIO_DIR_OUTPUT,
                  WM_GPIO_ATTR_PULLHIGH); // HW reset
  tls_gpio_cfg (dcs_pin, WM_GPIO_DIR_OUTPUT, WM_GPIO_ATTR_FLOATING);
  digitalWrite (dcs_pin, HIGH); // Start HIGH for SCI en SDI
  digitalWrite (cs_pin, HIGH);
  delay (100);
  LOG ("\n");
  LOG ("Reset VS1053...\n");
  if (gpio_is_valid (rst_pin))
    {
      digitalWrite (rst_pin, LOW);
      delay (100);
      digitalWrite (rst_pin, HIGH);
      delay (100);
      VS1053_await_data_request ();
      // DREQ rises when initialization is complete. You should not send any
      // data or commands before that.
    }
  digitalWrite (dcs_pin, LOW); // Low & Low will bring reset pin low
  digitalWrite (cs_pin, LOW);
  delay (500);
  LOG ("End reset VS1053...\n");
  digitalWrite (dcs_pin, HIGH); // Back to normal again
  digitalWrite (cs_pin, HIGH);
  delay (500);
  // Init SPI in slow mode ( 0.2 MHz )
  VS1053_SPI = SPI_Settings (FCLK_SLOW_VS1053);
  // printDetails("Right after reset/startup");
  delay (20);
  // printDetails("20 msec after reset");
  if (VS1053_testComm ("Slow SPI,Testing VS1053 read/write registers...\n")
      || spi_di == WM_IO_PB_25)
    {
      // softReset();
      // Switch on the analog parts
      VS1053_writeRegister (SCI_AUDATA, 44101); // 44.1kHz stereo

      if (spi_fastest_speed)
        {
          VS1053_writeRegister (SCI_CLOCKF, HZ_TO_SC_FREQ (12288000)
                                                | SC_MULT_53_50X
                                                | SC_ADD_53_10X);
          // XTALI×5.0 clock settings multiplyer 5.0 = 20 MHz
          // SPI Clock to 6 MHz. Now you can set high speed SPI clock.
        }
      else
        {
          VS1053_writeRegister (SCI_CLOCKF, HZ_TO_SC_FREQ (12288000)
                                                | SC_MULT_53_30X
                                                | SC_ADD_53_10X);
          // Normal clock settings multiplyer 3.0 = 12.2 MHz
        }

      VS1053_await_data_request ();

      if (spi_fastest_speed)
        {
          VS1053_SPI = SPI_Settings (FCLK_SUPER_FAST_VS1053);
        }
      else
        {
          VS1053_SPI = SPI_Settings (
              FCLK_FAST_VS1053); // SPI Clock to 4 MHz. Now you can set high
                                 // speed SPI clock.
        }

      VS1053_writeRegister (SCI_MODE, _BV (SM_SDINEW) | _BV (SM_LINE1));
      VS1053_testComm (
          "Fast SPI, Testing VS1053 read/write registers again...\n");
      delay (10);
      VS1053_await_data_request ();
      endFillByte = VS1053_wram_read (0x1E06) & 0xFF;
      LOG ("endFillByte is %X\n", endFillByte);
      // printDetails("After last clocksetting") ;
      delay (100);
    }

  if (VS1053_getChipVersion () == 4 || spi_di != WM_IO_PB_03)
    { // Only perform an update if we really are using a VS1053, not. eg.
      // VS1003
      VS1053_loadDefaultVs1053Patches ();
    }

  VS1053_enableI2sOut (i2sRateOut);
}

void
VS1053_setVolume (uint8_t vol)
{
  // Set volume.  Both left and right.
  // Input value is 0..100.  100 is the loudest.
  uint8_t valueL, valueR; // Values to send to SCI_VOL

  curvol = vol; // Save for later use
  valueL = vol;
  valueR = vol;

  if (curbalance < 0)
    {
      valueR = MAX (0, vol + curbalance);
    }
  else if (curbalance > 0)
    {
      valueL = MAX (0, vol - curbalance);
    }

  valueL = map (valueL, 0, 100, 0xFE, 0x00); // 0..100% to left channel
  valueR = map (valueR, 0, 100, 0xFE, 0x00); // 0..100% to right channel

  VS1053_writeRegister (SCI_VOL,
                        (valueL << 8) | valueR); // Volume left and right
}

void
VS1053_setBalance (s8_t balance)
{
  if (balance > 100)
    {
      curbalance = 100;
    }
  else if (balance < -100)
    {
      curbalance = -100;
    }
  else
    {
      curbalance = balance;
    }
}

void
VS1053_setTone (uint8_t *rtone)
{ // Set bass/treble (4 nibbles)
  // Set tone characteristics.  See documentation for the 4 nibbles.
  uint16_t value = 0; // Value to send to SCI_BASS
  int i;              // Loop control

  for (i = 0; i < 4; i++)
    {
      value = (value << 4) | rtone[i]; // Shift next nibble in
    }
  VS1053_writeRegister (SCI_BASS, value); // Volume left and right
}

uint8_t
VS1053_getVolume ()
{ // Get the currenet volume setting.
  return curvol;
}

s8_t
VS1053_getBalance ()
{ // Get the currenet balance setting.
  return curbalance;
}

void
VS1053_startSong ()
{
  VS1053_sdi_send_fillers (10);
}

void
VS1053_playChunk (uint8_t *data, size_t len)
{
  VS1053_sdi_send_buffer (data, len);
}

void
VS1053_stopSong ()
{
  uint16_t modereg; // Read from mode register
  int i;            // Loop control

  VS1053_sdi_send_fillers (2052);
  delay (10);
  VS1053_writeRegister (SCI_MODE, _BV (SM_SDINEW) | _BV (SM_CANCEL));
  for (i = 0; i < 200; i++)
    {
      VS1053_sdi_send_fillers (32);
      modereg = VS1053_read_register (SCI_MODE); // Read status
      if ((modereg & _BV (SM_CANCEL)) == 0)
        {
          VS1053_sdi_send_fillers (2052);
          LOG ("Song stopped correctly after %d msec\n", i * 10);
          return;
        }
      delay (10);
    }
  printf ("Song stopped incorrectly!");
}

/**
 * VLSI datasheet: "SM_STREAM activates VS1053b’s stream mode. In this mode,
 * data should be sent with as even intervals as possible and preferable in
 * blocks of less than 512 bytes, and VS1053b makes every attempt to keep its
 * input buffer half full by changing its playback speed up to 5%. For best
 * quality sound, the average speed error should be within 0.5%, the bitrate
 * should not exceed 160 kbit/s and VBR should not be used. For details, see
 * Application Notes for VS10XX. This mode only works with MP3 and WAV files."
 */

void
VS1053_streamModeOn ()
{
  LOG ("Performing streamModeOn\n");
  VS1053_writeRegister (SCI_MODE, _BV (SM_SDINEW) | _BV (SM_STREAM));
  delay (10);
  VS1053_await_data_request ();
}

void
VS1053_streamModeOff ()
{
  LOG ("Performing streamModeOff\n");
  VS1053_writeRegister (SCI_MODE, _BV (SM_SDINEW));
  delay (10);
  VS1053_await_data_request ();
}

void
VS1053_printDetails (const char *header)
{
  //  uint16_t regbuf[16];
  uint8_t i;
  //(void)regbuf;

  LOG ("%s", header);
  LOG ("REG   Contents\n");
  LOG ("---   -----\n");
  for (i = 0; i <= SCI_num_registers; i++)
    {
      LOG ("%3X - %5X\n", i, VS1053_read_register (i));
    }
}

/**
 * An optional switch.
 * Most VS1053 modules will start up in MIDI mode. The result is that there is
 * no audio when playing MP3. You can modify the board, but there is a more
 * elegant way without soldering. No side effects for boards which do not need
 * this switch. It means you can call it just in case.
 *
 * Read more here: http://www.bajdi.com/lcsoft-vs1053-mp3-module/#comment-33773
 */
void
VS1053_switchToMp3Mode ()
{
  VS1053_wram_write (ADDR_REG_GPIO_DDR_RW, 3);   // GPIO DDR = 3
  VS1053_wram_write (ADDR_REG_GPIO_ODATA_RW, 0); // GPIO ODATA = 0
  delay (100);
  LOG ("Switched to mp3 mode\n");
  VS1053_softReset ();
}

void
VS1053_disableI2sOut ()
{
  VS1053_wram_write (ADDR_REG_I2S_CONFIG_RW, 0x0000);

  // configure GPIO0 4-7 (I2S) as input (default)
  // leave other GPIOs unchanged
  uint16_t cur_ddr = VS1053_wram_read (ADDR_REG_GPIO_DDR_RW);
  VS1053_wram_write (ADDR_REG_GPIO_DDR_RW, cur_ddr & ~0x00f0);
}

void
VS1053_enableI2sOut (enum VS1053_I2S_RATE i2sRate)
{
  if (i2sRate == VS1053_I2S_NONE)
    {
      return;
    }

  // configure GPIO0 4-7 (I2S) as output
  // leave other GPIOs unchanged
  uint16_t cur_ddr = VS1053_wram_read (ADDR_REG_GPIO_DDR_RW);
  VS1053_wram_write (ADDR_REG_GPIO_DDR_RW, cur_ddr | 0x00f0);

  uint16_t i2s_config = 0x000c; // Enable MCLK(3); I2S(2)
  switch (i2sRate)
    {
    case VS1053_I2S_RATE_192_KHZ:
      i2s_config |= 0x0002;
      break;
    case VS1053_I2S_RATE_96_KHZ:
      i2s_config |= 0x0001;
      break;
    default:
    case VS1053_I2S_RATE_48_KHZ:
      // 0x0000
      break;
    }

  VS1053_wram_write (ADDR_REG_I2S_CONFIG_RW, i2s_config);
}

/**
 * A lightweight method to check if VS1053 is correctly wired up (power supply
 * and connection to SPI interface).
 *
 * @return true if the chip is wired up correctly
 */
bool
VS1053_isChipConnected ()
{
  uint16_t status = VS1053_read_register (SCI_STATUS);

  return !(status == 0 || status == 0xFFFF);
}

/**
 * get the Version Number for the VLSI chip
 * VLSI datasheet: 0 for VS1001, 1 for VS1011, 2 for VS1002, 3 for VS1003, 4
 * for VS1053 and VS8053, 5 for VS1033, 7 for VS1103, and 6 for VS1063.
 */
uint16_t
VS1053_getChipVersion ()
{
  uint16_t status = VS1053_read_register (SCI_STATUS);

  LOG ("VS1053_getChipVersion = %d \n", ((status & 0x00F0) >> 4));

  return ((status & 0x00F0) >> 4);
}

/**
 * Provides current decoded time in full seconds (from SCI_DECODE_TIME register
 * value)
 *
 * When decoding correct data, current decoded time is shown in SCI_DECODE_TIME
 * register in full seconds. The user may change the value of this register.
 * In that case the new value should be written twice to make absolutely
 * certain that the change is not overwritten by the firmware. A write to
 * SCI_DECODE_TIME also resets the byteRate calculation.
 *
 * SCI_DECODE_TIME is reset at every hardware and software reset. It is no
 * longer cleared when decoding of a file ends to allow the decode time to
 * proceed automatically with looped files and with seamless playback of
 * multiple files. With fast playback (see the playSpeed extra parameter) the
 * decode time also counts faster. Some codecs (WMA and Ogg Vorbis) can also
 * indicate the absolute play position, see the positionMsec extra parameter in
 * section 10.11.
 *
 * @see VS1053b Datasheet (1.31) / 9.6.5 SCI_DECODE_TIME (RW)
 *
 * @return current decoded time in full seconds
 */
uint16_t
VS1053_getDecodedTime ()
{
  return VS1053_read_register (SCI_DECODE_TIME);
}

/**
 * Clears decoded time (sets SCI_DECODE_TIME register to 0x00)
 *
 * The user may change the value of this register. In that case the new value
 * should be written twice to make absolutely certain that the change is not
 * overwritten by the firmware. A write to SCI_DECODE_TIME also resets the
 * byteRate calculation.
 */
void
VS1053_clearDecodedTime ()
{
  VS1053_writeRegister (SCI_DECODE_TIME, 0x00);
  VS1053_writeRegister (SCI_DECODE_TIME, 0x00);
}

/**
 * Fine tune the data rate
 */
void
VS1053_adjustRate (long ppm2)
{
  VS1053_writeRegister (SCI_WRAMADDR, 0x1e07);
  VS1053_writeRegister (SCI_WRAM, ppm2);
  VS1053_writeRegister (SCI_WRAM, ppm2 >> 16);
  // oldClock4KHz = 0 forces  adjustment calculation when rate checked.
  VS1053_writeRegister (SCI_WRAMADDR, 0x5b1c);
  VS1053_writeRegister (SCI_WRAM, 0);
  // Write to AUDATA or CLOCKF checks rate and recalculates adjustment.
  VS1053_writeRegister (SCI_AUDATA, VS1053_read_register (SCI_AUDATA));
}

/**
 * Load a patch or plugin
 *
 * Patches can be found on the VLSI Website
 * http://www.vlsi.fi/en/support/software/vs10xxpatches.html
 *
 * Please note that loadUserCode only works for compressed plugins (file ending
 * .plg). To include them, rename them to file ending .h Please also note that,
 * in order to avoid multiple definitions, if you are using more than one
 * patch, it is necessary to rename the name of the array plugin[] and the name
 * of PLUGIN_SIZE to names of your choice. example: after renaming plugin[] to
 * plugin_myname[] and PLUGIN_SIZE to PLUGIN_MYNAME_SIZE the method is called
 * by player.loadUserCode(plugin_myname, PLUGIN_MYNAME_SIZE) It is also
 * possible to just rename the array plugin[] to a name of your choice example:
 * after renaming plugin[] to plugin_myname[] the method is called by
 * player.loadUserCode(plugin_myname,
 * sizeof(plugin_myname)/sizeof(plugin_myname[0]))
 */
void
VS1053_loadUserCode (const unsigned short *plugin, unsigned short plugin_size)
{
  int i = 0;
  while (i < plugin_size)
    {
      unsigned short addr, n, val;
      addr = plugin[i++];
      n = plugin[i++];
      if (n & 0x8000U)
        { /* RLE run, replicate n samples */
          n &= 0x7FFF;
          val = plugin[i++];
          while (n--)
            {
              VS1053_writeRegister (addr, val);
            }
        }
      else
        { /* Copy run, copy n samples */
          while (n--)
            {
              val = plugin[i++];
              VS1053_writeRegister (addr, val);
            }
        }
    }
}

/**
 * Load the latest generic firmware patch
 */
void
VS1053_loadDefaultVs1053Patches ()
{
  VS1053_loadUserCode (PATCHES, PATCHES_SIZE);
};

void
VS1053_sineTest (uint8_t n, uint16_t ms)
{
  VS1053_reset ();

  uint16_t mode = VS1053_read_register (SCI_MODE);
  mode |= _BV (SM_TESTS);
  VS1053_writeRegister (SCI_MODE, mode);
  VS1053_await_data_request ();

  union
  {
    uint8_t byte_full;
    struct bitFields
    {
      uint8_t S : 5;
      uint8_t F : 3;
    } bits;
  } un;
  un.byte_full = n;
  /*
  bits 7:5 Fs
  0 44100 Hz
  1 48000 Hz
  2 32000 Hz
  3 22050 Hz
  4 24000 Hz
  5 16000 Hz
  6 11025 Hz
  7 12000 Hz

  S=bits 4:0

  Hz = Fs*(S/128);
  */
  u16 Fs[] = { 44100, 48000, 32000, 22050, 24000, 16000, 11025, 12000 };
  LOG (" VS1053_sineTest, n=%d F=%d S=%d Hz=%d \n", n, un.bits.F, un.bits.S,
       (Fs[un.bits.F] * un.bits.S) / 128);

  uint8_t sine_start[8] = { 0x53, 0xEF, 0x6E, n, 0x00, 0x00, 0x00, 0x00 };
  uint8_t sine_stop[8] = { 0x45, 0x78, 0x69, 0x74, 0x00, 0x00, 0x00, 0x00 };

  SPI_writeBytes (sine_start, 8);
  delay (ms);
  SPI_writeBytes (sine_stop, 8);
}

#define VS1053_GPIO_DDR 0xC017 // GPIO Direction
#define VS1053_GPIO_IDATA 0xC018 // GPIO Values read from pins
#define VS1053_GPIO_ODATA 0xC019 // GPIO Values set to the pins

void
VS1053_GPIO_pinMode (uint8_t pin, uint8_t dir)
{
  if (pin > 7)
    return;

  VS1053_writeRegister (SCI_WRAMADDR, VS1053_GPIO_DDR);
  uint16_t ddr = VS1053_read_register (SCI_WRAM);

  if (dir == INPUT)
    ddr &= ~_BV (pin);
  if (dir == OUTPUT)
    ddr |= _BV (pin);

  VS1053_writeRegister (SCI_WRAMADDR, VS1053_GPIO_DDR);
  VS1053_writeRegister (SCI_WRAM, ddr);
}

void
VS1053_GPIO_digitalWrite_all_pin (uint8_t val)
{
  VS1053_writeRegister (SCI_WRAMADDR, VS1053_GPIO_ODATA);
  VS1053_writeRegister (SCI_WRAM, val);
}

void
VS1053_GPIO_digitalWrite_pin (uint8_t pin, uint8_t val)
{
  if (pin > 7)
    return;

  VS1053_writeRegister (SCI_WRAMADDR, VS1053_GPIO_ODATA);
  uint16_t pins = VS1053_read_register (SCI_WRAM);

  if (val == LOW)
    pins &= ~_BV (pin);
  if (val == HIGH)
    pins |= _BV (pin);

  VS1053_writeRegister (SCI_WRAMADDR, VS1053_GPIO_ODATA);
  VS1053_writeRegister (SCI_WRAM, pins);
}

uint16_t
VS1053_GPIO_digitalRead_all_pin (void)
{
  VS1053_writeRegister (SCI_WRAMADDR, VS1053_GPIO_IDATA);
  return VS1053_read_register (SCI_WRAM) & 0xFF;
}

bool
VS1053_GPIO_digitalRead_pin (uint8_t pin)
{
  if (pin > 7)
    return 0;

  VS1053_writeRegister (SCI_WRAMADDR, VS1053_GPIO_IDATA);
  uint16_t val = VS1053_read_register (SCI_WRAM);
  if (val & _BV (pin))
    return true;
  return false;
}

/************************/
