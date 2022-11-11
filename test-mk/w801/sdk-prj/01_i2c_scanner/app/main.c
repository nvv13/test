#include "wm_type_def.h"

#include "wm_i2c.h"
#include "wm_regs.h"

#include "wm_gpio.h"
#include "wm_pmu.h"

#include <stdio.h>
#include <string.h>

#include "wm_gpio_afsel.h"

#define I2C_FREQ (100000)

char char_buff[30];

uint8_t
i2c_send_addr (uint8_t addr)
{

  /*

    I2C->TX_RX = addr; // кладем адрес слейва в регистр данных I2C
    I2C->CR_SR = I2C_CR_STA | I2C_CR_WR
                 | I2C_CR_STO; // выдаем на шину START, запуcкаем передачу, по
                               // окончании передачи байта выдаем STOP
    while (I2C->CR_SR & I2C_SR_TIP)
      {
      }; // ждем окончания отправки
    return (I2C->CR_SR & I2C_SR_NAK); // если обнаружили NACK, возвращаем "1",
                                      // если ACK - то "0"

  */

  tls_reg_write32 (HR_I2C_TX_RX,
                   addr); // кладем адрес слейва в регистр данных I2C
  tls_reg_write32 (
      HR_I2C_CR_SR,
      I2C_CR_STA | I2C_CR_WR // выдаем на шину START, запуcкаем передачу, по
          | I2C_CR_STO // окончании передачи байта выдаем STOP
  );
  while (tls_reg_read32 (HR_I2C_CR_SR) & I2C_SR_TIP)
    ; // ждем окончания отправки

  return (tls_reg_read32 (HR_I2C_CR_SR)
          & I2C_SR_NAK); // если обнаружили NACK, возвращаем "1",
                         // если ACK - то "0"
}

void
UserMain (void)
{

  wm_i2c_scl_config (WM_IO_PA_01);
  wm_i2c_sda_config (WM_IO_PA_04);
  tls_i2c_init (I2C_FREQ);

  printf ("Start I2C Address scan...\n\r");
  for (uint8_t addr = 0x08; addr < 0xF0; addr += 2)
    {
      if (!i2c_send_addr (addr))
        {
          sprintf (char_buff, "0x%.2X addr ACK found!\n\r", addr);
          printf (char_buff);
        }
    }
  printf ("I2C Address scan finished \n\r");
  while (1)
    {
    };
}
