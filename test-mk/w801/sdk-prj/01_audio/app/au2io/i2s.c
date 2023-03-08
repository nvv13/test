
#include "i2s.h"

void i2s_driver_install (i2s_port_t m_i2s_num, i2s_config_t *m_i2s_config,
                         int a, int *f){};

void
i2s_set_sample_rates (i2s_port_t m_i2s_num, u32 sampRate)
{
  //
  return;
};

esp_err_t
i2s_set_pin (i2s_port_t m_i2s_num, i2s_pin_config_t *m_pin_config)
{
  return 0;
};

void
i2s_zero_dma_buffer (i2s_port_t m_i2s_num)
{
  //
  return;
};

esp_err_t
i2s_write (i2s_port_t m_i2s_num, const char *s32, size_t len,
           size_t *m_i2s_bytesWritten, u8 a)
{
  return 0;
};

u8
i2s_start (i2s_port_t i2s_num)
{
  return 0;
};

u8
i2s_stop (i2s_port_t i2s_num)
{
  return 0;
};

void i2s_driver_uninstall (i2s_port_t m_i2s_num){
  //
};
