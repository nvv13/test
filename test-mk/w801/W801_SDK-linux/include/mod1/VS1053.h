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

#ifdef __cplusplus
extern "C"
{
#endif

#include "ff.h"
#include "wm_io.h"
#include "wm_type_def.h"
#include "wm_psram.h"

  enum VS1053_I2S_RATE
  {
    VS1053_I2S_RATE_192_KHZ,
    VS1053_I2S_RATE_96_KHZ,
    VS1053_I2S_RATE_48_KHZ
  };

#ifndef GPIO_UNDEF
#define GPIO_UNDEF -1
#endif

#ifndef gpio_is_valid
#define gpio_is_valid(pin) ((pin != GPIO_UNDEF))
#endif

#define vs1053_chunk_size 32

  typedef struct
  {

    enum tls_io_name rst_pin; // -> rst Reset pin? not use GPIO_UNDEF
    enum tls_io_name
        cs_pin; // -> cs Serial Control Interface (SCI) Chip Select pin
    enum tls_io_name
        dcs_pin; // -> dcs Serial Data Interface (SDI) Chip Select pin
    enum tls_io_name dreq_pin; // <- dreq Data Request pin DREQ is turned low
                               // when the stream buffer is too full

    enum tls_io_name spi_cs; /* . cs Chip Select pin, даже если не
                                используеться, надо определить? */
    enum tls_io_name spi_ck; /* -> clk Clock input pin -> */

    enum tls_io_name spi_di; /* miso MISO (Master In Slave Out) pin <- */
                             /**
                              * config the pins used for spi di
                              * WM_IO_PB_00 - не работает,
                              * WM_IO_PB_03 - работает!
                              * WM_IO_PB_16 only for 56pin - не работает, мешает светодиод подключенный
                              * к                          данному контакту на макетке                          WM_IO_PB_25 only
                              * for                          56pin - не работает,                          мешает светодиод
                              * подключенный к данному контакту на                          макетке
                              */

    enum tls_io_name spi_do; /* mosi MOSI (Master Out Slave In) pin -> */

    /* далее настройки для VS1053_PlayHttpMp3 */
    u16 no_psram_BufferSize;// подойдет 4000, более - программа начнет глючить
    u32 psram_BufferSize;   // подойдет 26400 более не надо! лучьше не становится

    u8 psram_config;//0 или 1 
    psram_mode_t psram_mode;// делай PSRAM_SPI, PSRAM_QPI - так и не работает
    u8 psram_frequency_divider;//2 - хорошо работает для ESP-PSRAM64H 
    u8 psram_tCPH;//2 - хорошо работает для ESP-PSRAM64H 
    u8 psram_BURST;//1 - хорошо работает для ESP-PSRAM64H 
    u16 psram_OVERTIMER;//2 - хорошо работает для ESP-PSRAM64H 

    u8 load_buffer_debug;//0 , 1 - выводит инфу по заполнению f или +, и опусташению буффера -

    u8 spi_fastest_speed;//0 - 4 MHz работает на большенстве плат, 1 - 6 MHz это может приводить к срыву передачи на некоторых платах
    
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
  // void VS1053_enableI2sOut(VS1053_I2S_RATE i2sRate =
  // VS1053_I2S_RATE_48_KHZ);
  void VS1053_enableI2sOut (enum VS1053_I2S_RATE i2sRate);

  // Checks whether the VS1053 chip is connected and is able to exchange data
  // to the ESP
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

#define N_Hz_1000 68
#define N_Hz_2000 72
#define N_Hz_3000 40
#define N_Hz_5000 84
#define N_Hz_9000 56
  void VS1053_sineTest (uint8_t n, uint16_t ms);

  typedef enum VS1053_status
  {
    VS1053_NONE = 0,
    VS1053_HW_INIT = 1,
    VS1053_PLAY = 2,
    VS1053_END_FILE = 3,
    VS1053_STOP = 4,
    VS1053_QUERY_TO_STOP = 5,
    VS1053_PLAY_BUF = 6,
  } _VS1053_status;

  extern volatile enum VS1053_status my_sost;

  enum VS1053_status VS1053_status_get_status (void);

  FRESULT VS1053_PlayMp3 (char *filename);
  FRESULT VS1053_PlayFlac (char *filename);

  void VS1053_stop_PlayMP3 (void);

  FRESULT VS1053_PlayHttpMp3 (const char *Uri);

#ifdef __cplusplus
}
#endif

#endif
