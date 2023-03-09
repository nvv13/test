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

#include "wm_type_def.h"

enum VS1053_I2S_RATE
{
  VS1053_I2S_RATE_192_KHZ,
  VS1053_I2S_RATE_96_KHZ,
  VS1053_I2S_RATE_48_KHZ
};

  typedef struct
  {

    enum tls_io_name cs_pin; 
    enum tls_io_name dcs_pin;
    enum tls_io_name dreq_pin;

    enum tls_io_name spi_cs; /* */
    enum tls_io_name spi_ck; /* */
    enum tls_io_name spi_di; /* даже если не используеться, надо определить? */
    enum tls_io_name spi_do; /* */

  } libVS1053_t;

// Constructor.  Only sets pin values.  Doesn't touch the chip.  Be sure to
// call begin()!
void VS1053_VS1053 (libVS1053_t* set_pin);

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
void VS1053_writeRegister (uint8_t _reg, uint16_t _value);

// Load a patch or plugin to fix bugs and/or extend functionality.
// For more info about patches see
// http://www.vlsi.fi/en/support/software/vs10xxpatches.html
void VS1053_loadUserCode (const unsigned short *plugin,
                          unsigned short plugin_size);

// Loads the latest generic firmware patch.
void VS1053_loadDefaultVs1053Patches ();

#endif
