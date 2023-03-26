#include "types.h"

typedef struct
{
/*  __IO uint16_t CR1;
  uint16_t  RESERVED0;
  __IO uint16_t CR2;
  uint16_t  RESERVED1;
  __IO uint16_t SR;
  uint16_t  RESERVED2;
  __IO uint16_t DR;
  uint16_t  RESERVED3;
  __IO uint16_t CRCPR;
  uint16_t  RESERVED4;
  __IO uint16_t RXCRCR;
  uint16_t  RESERVED5;
  __IO uint16_t TXCRCR;
  uint16_t  RESERVED6;
  __IO uint16_t I2SCFGR;
  uint16_t  RESERVED7;
  __IO uint16_t I2SPR;
  uint16_t  RESERVED8;  
*/
 int a;
} SPI_TypeDef;

SPI_TypeDef* SPI1;

// Процедура инициализации spi (SPI1 или SPI2) в режиме master
void spim_init(SPI_TypeDef* SPIx, uint8_t FastSpeed, uint8_t WordLen);

// Процедура отправляет массив 8-битных слов
void SPI_send8b(SPI_TypeDef* SPIx, uint8_t *pBuff, uint16_t Len);