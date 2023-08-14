//**********************************************************************************************
#include "wm_i2c.h"

static void
Wire_begin (enum tls_io_name i2c_scl, enum tls_io_name i2c_sda)
{
  wm_i2c_scl_config (i2c_scl);
  wm_i2c_sda_config (i2c_sda);
  tls_i2c_init (100000);
}

static u8
Wire_beginTransmission (u8 AddresI2C)
{
  tls_reg_write32 (HR_I2C_TX_RX,
                   (AddresI2C << 1)); // заносим в регистр данных адрес слейва

  tls_reg_write32 (
      HR_I2C_CR_SR,
      I2C_CR_STA | I2C_CR_WR); // включаем модуль на передачу и выдаем START

  while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_TIP)
    ; // ждем окончания передачи

  if (tls_reg_read32 (HR_I2C_CR_SR)
      & I2C_SR_NAK) // если по окончанию передачи байта слейв не ответил
    {
      tls_reg_write32 (HR_I2C_CR_SR, I2C_CR_STO); // останавливаем обмен,
      while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_BUSY)
        ; // ожидаем освобождения шины
      printf ("wire_beginTransmission error ASK AddresI2C= 0x%.2X \n",
              AddresI2C);
      return 1; // возвращаем код ошибки "1"
    }           // если есть ответ от слейва

  return 0; // возвращаем "0" - передача успешна
}

static u8
Wire_write_buf (u8 *cmdI2C, u8 u8_Len)
{
  for (u8 u8_index = 0; u8_index < u8_Len; u8_index++)
    {
      u8 data = cmdI2C[u8_index];

      tls_reg_write32 (HR_I2C_TX_RX,
                       data); // заносим в регистр данных байт на отправку
      if (u8_index == (u8_Len - 1))
        {
          tls_reg_write32 (HR_I2C_CR_SR, // I2C_CR_NAK |
                           I2C_CR_WR
                               | I2C_CR_STO); // передаем байт и по окончании
                                              // передачи - STOP
#ifdef LCD_SERIAL_DEBUG
          printf ("i2c wire_write I2C_CR_STO data= 0x%.2X \n", data);
#endif
        }
      else
        {
          tls_reg_write32 (
              HR_I2C_CR_SR, // I2C_CR_NAK |
              I2C_CR_WR); // передаем байт и по окончании передачи - STOP
#ifdef LCD_SERIAL_DEBUG
          printf ("i2c wire_write data= 0x%.2X \n", data);
#endif
        }

      // while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_BUSY)
      //  ;       // ожидаем освобождения шины
      while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_TIP)
        ; // ждем окончания передачи

      if (tls_reg_read32 (HR_I2C_CR_SR)
          & I2C_SR_NAK) // если по окончанию передачи байта слейв не ответил
        {
          tls_reg_write32 (HR_I2C_CR_SR, I2C_CR_STO); // останавливаем обмен,
          while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_BUSY)
            ; // ожидаем освобождения шины
          printf ("i2c wire_write error ASK data= 0x%.2X \n", data);
          return 1; // возвращаем код ошибки "1"
        }           // если есть ответ от слейва
    }
  return 0; // возвращаем "0" - передача успешна
}
static u8
Wire_write (u8 cmdI2C)
{
  return Wire_write_buf (&cmdI2C, 1);
}

static void
Wire_read_buf (u8 AddresI2C, u8 *buf, u8 len)
{
  tls_i2c_write_byte ((AddresI2C << 1) | 0x01, 1);
  tls_i2c_wait_ack ();
  while (len > 1)
    {
      *buf++ = tls_i2c_read_byte (1, 0);
      // printf("\nread byte=%x\n",*(pBuffer - 1));
      len--;
    }
  *buf = tls_i2c_read_byte (0, 1);
}

static u8
Wire_endTransmission (void)
{
  if (tls_reg_read32 (HR_I2C_CR_SR)
      & I2C_SR_NAK) // если по окончанию передачи байта слейв не ответил
    {
      tls_reg_write32 (HR_I2C_CR_SR, I2C_CR_STO); // останавливаем обмен,
      while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_BUSY)
        ; // ожидаем освобождения шины
      printf ("wire_endTransmission error ASK\n");
      return 1; // возвращаем код ошибки "1"
    }           // если есть ответ от слейва

  return 0; // возвращаем "0" - передача успешна
}

//************************************************************************8
