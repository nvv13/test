#ifndef AT24C32_UTIL_H
#define AT24C32_UTIL_H

#ifdef __cplusplus
extern "C"
{
#endif

  int
  at24c_ReadByte (const at24cxxx_t *dev, int iPosition, u8 *u8_value)
  {
    int check = at24cxxx_read_byte (dev, iPosition, u8_value);
    if (check < 0)
      {
        PoCShell->oUsart->SendPSZstring ("[FAILURE] read_byte");
        PoCShell->oUsart->SendIntToStr (check);
        PoCShell->oUsart->SendPSZstring ("\r\n");
        return 1;
      }
    else
      {
        PoCShell->oUsart->SendPSZstring ("[SUCCESS] at24cxxx_read_byte\r\n");
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
        PoCShell->oUsart->SendPSZstring ("[FAILURE] write_byte");
        PoCShell->oUsart->SendIntToStr (check);
        PoCShell->oUsart->SendPSZstring ("\r\n");
        return 1;
      }
    else
      {
        PoCShell->oUsart->SendPSZstring ("[SUCCESS] at24cxxx_write_byte\r\n");
      }
    return 0;
  }

#ifdef __cplusplus
}
#endif

#endif /* AT24C32_UTIL_H */
