#include "at24c32_util.h"

int
at24c_ReadByte (const at24cxxx_t *dev, int iPosition, u8 *u8_value)
{
  int check = at24cxxx_read_byte (dev, iPosition, u8_value);
  if (check < 0)
    {
      printf ("[FAILURE] at24cxxx_read_byte: %d\n", check);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_read_byte");
    }
  return 0;
}

int
at24c_WriteByte (const at24cxxx_t *dev, int iPosition, u8 u8_value)
{
  /* Test: Write/Read Byte */
  int check = at24cxxx_write_byte (dev, iPosition, u8_value);
  if (check != AT24CXXX_OK)
    {
      printf ("[FAILURE] at24cxxx_write_byte: %d\n", check);
      return 1;
    }
  else
    {
      puts ("[SUCCESS] at24cxxx_write_byte");
    }
  return 0;
}
