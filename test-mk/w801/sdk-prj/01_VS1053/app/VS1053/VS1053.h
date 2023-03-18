/**
 * This is a driver library for VS1053 MP3 Codec Breakout
 * (Ogg Vorbis / MP3 / AAC / WMA / FLAC / MIDI Audio Codec Chip).
 * Adapted for Espressif ESP8266 and ESP32 boards.
 *
 * version 1.0.1
 *
 * Licensed under GNU GPLv3 <http://gplv3.fsf.org/>
 * Copyright © 2017
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

#ifndef VS1053_H
#define VS1053_H

#include "ff.h"
#include "wm_type_def.h"

enum VS1053_I2S_RATE
{
  VS1053_I2S_RATE_192_KHZ,
  VS1053_I2S_RATE_96_KHZ,
  VS1053_I2S_RATE_48_KHZ
};

#define GPIO_UNDEF -1
#define gpio_is_valid(pin) ((pin != GPIO_UNDEF))
typedef struct
{

  enum tls_io_name rst_pin; // -> rst Reset pin? not use GPIO_UNDEF
  enum tls_io_name
      cs_pin; // -> cs Serial Control Interface (SCI) Chip Select pin
  enum tls_io_name
      dcs_pin; // -> dcs Serial Data Interface (SDI) Chip Select pin
  enum tls_io_name dreq_pin; // <- dreq Data Request pin DREQ is turned low
                             // when the stream buffer is too full

  enum tls_io_name spi_cs; /* . cs Chip Select pin, даже если не используеться,
                              надо определить? */
  enum tls_io_name spi_ck; /* -> clk Clock input pin -> */

  enum tls_io_name spi_di; /* miso MISO (Master In Slave Out) pin <- */
                           /**
                            * config the pins used for spi di
                            * WM_IO_PB_00 - не работает,
                            * WM_IO_PB_03 - работает!
                            * WM_IO_PB_16 only for 56pin - не работает, мешает светодиод подключенный к
                            * данному контакту на макетке                          WM_IO_PB_25 only for
                            * 56pin - не работает,                          мешает светодиод подключенный к данному контакту на
                            * макетке
                            */

  enum tls_io_name spi_do; /* mosi MOSI (Master Out Slave In) pin -> */

} libVS1053_t;

// Constructor.  Only sets pin values.  Doesn't touch the chip.  Be sure to
// call begin()!
void VS1053_VS1053 (libVS1053_t *set_pin);

// Begin operation.  Sets pins correctly, and prepares SPI bus.
void VS1053_begin ();

// Prepare to start playing. Call this each time a new song starts
void VS1053_startSong ();

// Play a chunk of data.  Copies the data to the chip.  Blocks until complete
void VS1053_playChunk (uint8_t *data, size_t len);

// Finish playing a song. Call this after the last playChunk call
void VS1053_stopSong ();

// Set the player volume.Level from 0-100, higher is louder
void VS1053_setVolume (uint8_t vol);

// Adjusting the left and right volume balance, higher to enhance the right
// side, lower to enhance the left side.
void VS1053_setBalance (s8_t balance);

// Set the player baas/treble, 4 nibbles for treble gain/freq and bass
// gain/freq
void VS1053_setTone (uint8_t *rtone);

// Get the currenet volume setting, higher is louder
uint8_t VS1053_getVolume ();

// Get the currenet balance setting (-100..100)
s8_t VS1053_getBalance ();

// Print configuration details to serial output.
void VS1053_printDetails (const char *header);

// Do a soft reset
void VS1053_softReset ();

// Do a HW + Soft reset
void VS1053_reset (void);

// Test communication with module
bool VS1053_testComm (const char *header);

// Fine tune the data rate
void VS1053_adjustRate (long ppm2);

// Streaming Mode On
void VS1053_streamModeOn ();

// Default: Streaming Mode Off
void VS1053_streamModeOff ();

// An optional switch preventing the module starting up in MIDI mode
void VS1053_switchToMp3Mode ();

// disable I2S output; this is the default state
void VS1053_disableI2sOut ();

// enable I2S output (GPIO4=LRCLK/WSEL; GPIO5=MCLK; GPIO6=SCLK/BCLK;
// GPIO7=SDATA/DOUT)
// void VS1053_enableI2sOut(VS1053_I2S_RATE i2sRate = VS1053_I2S_RATE_48_KHZ);
void VS1053_enableI2sOut (enum VS1053_I2S_RATE i2sRate);

// Checks whether the VS1053 chip is connected and is able to exchange data to
// the ESP
bool VS1053_isChipConnected ();

// gets Version of the VLSI chip being used
uint16_t VS1053_getChipVersion ();

// Provides SCI_DECODE_TIME register value
uint16_t VS1053_getDecodedTime ();

// Clears SCI_DECODE_TIME register (sets 0x00)
void VS1053_clearDecodedTime ();

// Writes to VS10xx's SCI (serial command interface) SPI bus.
// A low level method which lets users access the internals of the VS1053.
// void VS1053_writeRegister (uint8_t _reg, uint16_t _value);

// Load a patch or plugin to fix bugs and/or extend functionality.
// For more info about patches see
// http://www.vlsi.fi/en/support/software/vs10xxpatches.html
void VS1053_loadUserCode (const unsigned short *plugin,
                          unsigned short plugin_size);

// Loads the latest generic firmware patch.
void VS1053_loadDefaultVs1053Patches ();

void VS1053_GPIO_pinMode (uint8_t pin, uint8_t dir);

void VS1053_GPIO_digitalWrite_all_pin (uint8_t val);

void VS1053_GPIO_digitalWrite_pin (uint8_t pin, uint8_t val);

uint16_t VS1053_GPIO_digitalRead_all_pin (void);

bool VS1053_GPIO_digitalRead_pin (uint8_t pin);

#define N_Hz_86 193
#define N_Hz_93 225
#define N_Hz_125 161
#define N_Hz_172 97
#define N_Hz_187 129
#define N_Hz_250 65
#define N_Hz_258 195
#define N_Hz_281 227
#define N_Hz_344 1
#define N_Hz_375 33
#define N_Hz_430 197
#define N_Hz_468 229
#define N_Hz_500 66
#define N_Hz_516 99
#define N_Hz_562 131
#define N_Hz_602 199
#define N_Hz_625 165
#define N_Hz_656 231
#define N_Hz_689 2
#define N_Hz_750 34
#define N_Hz_775 201
#define N_Hz_843 233
#define N_Hz_861 101
#define N_Hz_875 167
#define N_Hz_937 133
#define N_Hz_947 203
#define N_Hz_1000 68
#define N_Hz_1031 235
#define N_Hz_1033 3
#define N_Hz_1119 205
#define N_Hz_1125 35
#define N_Hz_1205 103
#define N_Hz_1218 237
#define N_Hz_1250 69
#define N_Hz_1291 207
#define N_Hz_1312 135
#define N_Hz_1375 171
#define N_Hz_1378 4
#define N_Hz_1406 239
#define N_Hz_1464 209
#define N_Hz_1500 36
#define N_Hz_1550 105
#define N_Hz_1593 241
#define N_Hz_1625 173
#define N_Hz_1636 211
#define N_Hz_1687 137
#define N_Hz_1722 5
#define N_Hz_1750 71
#define N_Hz_1781 243
#define N_Hz_1808 213
#define N_Hz_1875 37
#define N_Hz_1894 107
#define N_Hz_1968 245
#define N_Hz_1981 215
#define N_Hz_2000 72
#define N_Hz_2062 139
#define N_Hz_2067 6
#define N_Hz_2125 177
#define N_Hz_2153 217
#define N_Hz_2156 247
#define N_Hz_2239 109
#define N_Hz_2250 38
#define N_Hz_2325 219
#define N_Hz_2343 249
#define N_Hz_2375 179
#define N_Hz_2411 7
#define N_Hz_2437 141
#define N_Hz_2497 221
#define N_Hz_2500 74
#define N_Hz_2531 251
#define N_Hz_2583 111
#define N_Hz_2625 39
#define N_Hz_2670 223
#define N_Hz_2718 253
#define N_Hz_2750 75
#define N_Hz_2756 8
#define N_Hz_2812 143
#define N_Hz_2875 183
#define N_Hz_2906 255
#define N_Hz_2928 113
#define N_Hz_3000 40
#define N_Hz_3100 9
#define N_Hz_3125 185
#define N_Hz_3187 145
#define N_Hz_3250 77
#define N_Hz_3273 115
#define N_Hz_3375 41
#define N_Hz_3445 10
#define N_Hz_3500 78
#define N_Hz_3562 147
#define N_Hz_3617 117
#define N_Hz_3625 189
#define N_Hz_3750 42
#define N_Hz_3789 11
#define N_Hz_3875 191
#define N_Hz_3937 149
#define N_Hz_3962 119
#define N_Hz_4000 80
#define N_Hz_4125 43
#define N_Hz_4134 12
#define N_Hz_4250 81
#define N_Hz_4306 121
#define N_Hz_4312 151
#define N_Hz_4478 13
#define N_Hz_4500 44
#define N_Hz_4651 123
#define N_Hz_4687 153
#define N_Hz_4750 83
#define N_Hz_4823 14
#define N_Hz_4875 45
#define N_Hz_4995 125
#define N_Hz_5000 84
#define N_Hz_5062 155
#define N_Hz_5167 15
#define N_Hz_5250 46
#define N_Hz_5340 127
#define N_Hz_5437 157
#define N_Hz_5500 86
#define N_Hz_5512 16
#define N_Hz_5625 47
#define N_Hz_5750 87
#define N_Hz_5812 159
#define N_Hz_5857 17
#define N_Hz_6000 48
#define N_Hz_6201 18
#define N_Hz_6250 89
#define N_Hz_6375 49
#define N_Hz_6500 90
#define N_Hz_6546 19
#define N_Hz_6750 50
#define N_Hz_6890 20
#define N_Hz_7000 92
#define N_Hz_7125 51
#define N_Hz_7235 21
#define N_Hz_7250 93
#define N_Hz_7500 52
#define N_Hz_7579 22
#define N_Hz_7750 95
#define N_Hz_7875 53
#define N_Hz_7924 23
#define N_Hz_8250 54
#define N_Hz_8268 24
#define N_Hz_8613 25
#define N_Hz_8625 55
#define N_Hz_8957 26
#define N_Hz_9000 56
#define N_Hz_9302 27
#define N_Hz_9375 57
#define N_Hz_9646 28
#define N_Hz_9750 58
#define N_Hz_9991 29
#define N_Hz_10125 59
#define N_Hz_10335 30
#define N_Hz_10500 60
#define N_Hz_10680 31
#define N_Hz_10875 61
#define N_Hz_11250 62
#define N_Hz_11625 63
void VS1053_sineTest (uint8_t n, uint16_t ms);

typedef enum VS1053_status
{
  VS1053_NONE = 0,
  VS1053_HW_INIT = 1,
  VS1053_PLAY = 2,
  VS1053_END_FILE = 3,
  VS1053_STOP = 4,
  VS1053_QUERY_TO_STOP = 5,
} _VS1053_status;

FRESULT VS1053_PlayMp3 (char *filename);

enum VS1053_status VS1053_status_get_status (void);

void VS1053_stop_PlayMP3 (void);

#endif
