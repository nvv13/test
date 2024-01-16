//**********************************************************************************************
#include "wm_i2c.h"

static void
HWire_init (i2c_t dev)
{
  wm_i2c_scl_config (dev->i2c_scl);
  wm_i2c_sda_config (dev->i2c_sda);
  tls_i2c_init (dev->i2c_freq);
}


static int HWire_read_regs(i2c_t dev, uint16_t addr, uint16_t reg,
                  void *data, size_t len, uint8_t flags)
{
  tls_i2c_write_byte ((addr << 1) | 0x00, 1);
  tls_i2c_wait_ack ();
  tls_i2c_write_byte (reg, 0);
  tls_i2c_wait_ack ();
  tls_i2c_write_byte ((addr << 1) | 0x01, 1);
  tls_i2c_wait_ack ();
  u8* buf=(u8*)data;
  while (len > 1)
    {
      *buf++ = tls_i2c_read_byte (1, 0);
      // printf("\nread byte=%x\n",*(pBuffer - 1));
      len--;
    }
  *buf = tls_i2c_read_byte (0, 1);
  return 0;
}


static int HWire_write_regs(i2c_t dev, uint16_t addr, uint16_t reg,
                  const void *data, size_t len, uint8_t flags)
{
  tls_i2c_write_byte ((addr << 1) | 0x00, 1);
  if (tls_i2c_wait_ack () != WM_SUCCESS)
    {
      tls_i2c_stop ();
      return 1;
    }
  tls_i2c_write_byte (reg, 0);
  tls_i2c_wait_ack ();
  for (u16 u8_index = 0; u8_index < len; u8_index++)
    {
      u8 dat = ((u8*)data)[u8_index];
      tls_i2c_write_byte (dat, 0);
      if (tls_i2c_wait_ack () != WM_SUCCESS)
        {
          tls_i2c_stop ();
          return 1;
        }
    }
  tls_i2c_stop ();
  return 0;
}

//************************************************************************8
