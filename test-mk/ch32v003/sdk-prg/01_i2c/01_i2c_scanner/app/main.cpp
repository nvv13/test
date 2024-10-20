
#include "ch32v00x_dbgmcu.h"
#include "debug.h"

#include "CBlink.hpp"

/*********************************************************************
 * @fn      IIC_Init
 *
 * @brief   Initializes the IIC peripheral.
 *
 * @return  none
 */

void
IIC_Init (u32 bound, u16 address)
{
  GPIO_InitTypeDef GPIO_InitStructure = { 0 };
  I2C_InitTypeDef I2C_InitTSturcture = { 0 };

  RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd (RCC_APB1Periph_I2C1, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init (GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init (GPIOC, &GPIO_InitStructure);

  I2C_InitTSturcture.I2C_ClockSpeed = bound;
  I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
  I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitTSturcture.I2C_OwnAddress1 = address;
  I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
  I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_Init (I2C1, &I2C_InitTSturcture);

  I2C_Cmd (I2C1, ENABLE);
}

char char_buff[30];

uint32_t
i2c_send_addr (uint8_t addr)
{

  while (I2C_GetFlagStatus (I2C1, I2C_FLAG_BUSY) != RESET)
    ;
  I2C_GenerateSTART (I2C1, ENABLE);

  while (!I2C_CheckEvent (I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    ;
  I2C_Send7bitAddress (I2C1, addr, I2C_Direction_Transmitter);

  I2C_CheckEvent (I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
  Delay_Us (200);
  uint32_t u83 = I2C_GetLastEvent (I2C1);

  I2C_GenerateSTOP (I2C1, ENABLE);

  return u83;
}

extern "C" int
main (void)
{

  SystemCoreClockUpdate ();
  Delay_Init ();

  USART_Printf_Init (115000);

  printf ("\r\nSystemClk:%d\r\n", SystemCoreClock);
  printf ("ChipID:%08x\r\n", DBGMCU_GetCHIPID ());

  IIC_Init (100000, 0x00);
  printf ("Start I2C Address scan...\n\r");
  bool l_find = false;
  for (uint8_t addr = 0x08; addr < 0xF0; addr += 2)
    {
      uint32_t res = i2c_send_addr (addr);
      if (res != 0x30400)
        {
          sprintf (char_buff,
                   "0x%.2X addr ACK found! = 0x%.2X      "
                   "flag2<<16|flag1=0x%.4X\n\r",
                   addr, addr >> 1, res);
          printf (char_buff);
          l_find = true;
        }
    }
  printf ("I2C Address scan finished \n\r");

  CBlink ob_Blink1 = CBlink (GPIOA, GPIO_Pin_1, GPIO_Speed_50MHz);

  while (1)
    {
      if (l_find)
        Delay_Ms (200);
      else
        Delay_Ms (1000);
      ob_Blink1.Toggle ();
    }
}
