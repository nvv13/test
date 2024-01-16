//**********************************************************************************************
#include "wm_i2c.h"

static void
HWire_init (enum tls_io_name i2c_scl, enum tls_io_name i2c_sda, u32 i2c_freq)
{
  wm_i2c_scl_config (i2c_scl);
  wm_i2c_sda_config (i2c_sda);
  tls_i2c_init (i2c_freq);
}

static void HWire_read_buf(u8 addr,u8 reg, u8 *buf, u8 len)
{
  tls_i2c_write_byte ((addr << 1) | 0x00, 1);
  tls_i2c_wait_ack ();
  tls_i2c_write_byte (reg, 0);
  tls_i2c_wait_ack ();
  tls_i2c_write_byte ((addr << 1) | 0x01, 1);
  tls_i2c_wait_ack ();
  while (len > 1)
    {
      *buf++ = tls_i2c_read_byte (1, 0);
      // printf("\nread byte=%x\n",*(pBuffer - 1));
      len--;
    }
  *buf = tls_i2c_read_byte (0, 1);
}

static u8 HWire_write_buf(u8 addr,u8 reg,u8 *buf, u8 u8_Len)
{
  tls_i2c_write_byte ((addr << 1) | 0x00, 1);
  if(tls_i2c_wait_ack ()!=WM_SUCCESS)
    {
      tls_i2c_stop();
      return 1;
    }
  tls_i2c_write_byte (reg, 0);
  tls_i2c_wait_ack ();
  for (u8 u8_index = 0; u8_index < u8_Len; u8_index++)
    {
       u8 data = buf[u8_index];
       tls_i2c_write_byte (data, 0);
       if(tls_i2c_wait_ack ()!=WM_SUCCESS)
         {
           tls_i2c_stop();
           return 1;
         }
    }
  tls_i2c_stop();
  return 0;
}

//************************************************************************8
