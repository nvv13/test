#include "wm_gpio.h"
#include "wm_gpio_afsel.h"

#include "wm_hostspi.h"

#include "spim.h"

// Процедура инициализации spi (SPI1 или SPI2) в режиме master
void spim_init(SPI_TypeDef* SPIx, uint8_t FastSpeed, uint8_t WordLen)
{
      /*MASTER SPI configuratioin*/
      wm_spi_cs_config (WM_IO_PB_14);
      wm_spi_ck_config (WM_IO_PB_15);
      wm_spi_di_config (WM_IO_PB_16);
      wm_spi_do_config (WM_IO_PB_17);
      // printf (
      //"MASTER SPI configuratioin cs--PB14, ck--PB15, di--PB16,
      // do--PB17;\r\n");
      tls_spi_trans_type (SPI_DMA_TRANSFER); // byte , word, dma
      tls_spi_setup (TLS_SPI_MODE_0,
                     TLS_SPI_CS_HIGH,                   // TLS_SPI_CS_LOW,
                     50000000
      );

}

// Процедура отправляет массив 8-битных слов
void SPI_send8b(SPI_TypeDef* SPIx, uint8_t *pBuff, uint16_t Len)
{
     tls_spi_write (pBuff, Len);
}