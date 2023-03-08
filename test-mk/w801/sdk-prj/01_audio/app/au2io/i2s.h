#pragma once

#if defined(__cplusplus)
extern "C"
{
#endif

#include "stddef.h"
#include "wm_type_def.h"

#define I2S_NUM_0 0
#define I2S_PIN_NO_CHANGE 0

#define I2S_BCLK 1
#define I2S_LRC 2
#define I2S_DOUT 3

  typedef enum
  {
    I2S_COMM_FORMAT_I2S = 0,
    I2S_COMM_FORMAT_I2S_LSB = 1,
    I2S_COMM_FORMAT_I2S_MSB = 2,
  } i2s_comm_format_t;

#define I2S_BITS_PER_SAMPLE_16BIT 16
#define I2S_CHANNEL_FMT_RIGHT_LEFT 1
#define ESP_INTR_FLAG_LEVEL1 1
//#define APLL_DISABLE 1 // must be disabled in V2.0.1-RC1
// I2S_PIN_NO_CHANGE
#define I2S_MODE_MASTER 1
#define I2S_MODE_TX 1

  typedef struct //         m_i2s_config = {}; // stores values for I2S driver
  {
    i2s_comm_format_t communication_format;
    u32 sample_rate;
    u8 bits_per_sample;
    u8 channel_format;
    u8 intr_alloc_flags; // interrupt priority
    u8 dma_buf_count;
    u16 dma_buf_len;
    u8 use_apll;             // must be disabled in V2.0.1-RC1
    bool tx_desc_auto_clear; // new in V1.0.1
    u8 fixed_mclk;
    u8 mode;
  } i2s_config_t;

  typedef struct //      m_pin_config = {};
  {
    u8 bck_io_num;
    u8 ws_io_num;
    u8 data_out_num;
    u8 data_in_num;
    u8 mck_io_num;
  } i2s_pin_config_t;

  // typedef struct
  //{
  //}i2s_port_t;

  //#define i2s_port_t u8
  typedef u8 i2s_port_t;

#define esp_err_t int
#define i2s_mode_t u8

  void i2s_driver_uninstall (i2s_port_t m_i2s_num);

  void i2s_driver_install (i2s_port_t m_i2s_num, i2s_config_t *m_i2s_config,
                           int a, int *f);

  void i2s_set_sample_rates (i2s_port_t m_i2s_num, u32 sampRate);

  esp_err_t i2s_set_pin (i2s_port_t m_i2s_num, i2s_pin_config_t *m_pin_config);

  void i2s_zero_dma_buffer (i2s_port_t m_i2s_num);

  esp_err_t i2s_write (i2s_port_t m_i2s_num, const char *s32, size_t len,
                       size_t *m_i2s_bytesWritten, u8 a);

  u8 i2s_start (i2s_port_t i2s_num);
  u8 i2s_stop (i2s_port_t i2s_num);

#if defined(__cplusplus)
}
#endif
