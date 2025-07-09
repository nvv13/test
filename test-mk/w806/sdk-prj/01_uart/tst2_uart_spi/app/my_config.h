#ifndef MY_CONFIG_H
#define MY_CONFIG_H


// 2x буффера spi и uart
#define DF_TRANSMIT_BUF_SIZE 1024

//буффер сообщений
#define DF_MESSAGEBUF_SIZE (DF_TRANSMIT_BUF_SIZE*2)

//пин для проверки состояния ввода данных (0-данные идут, 1-надо отправлять что
//уже принял)
#define DF_CHECK_STATE_FILL WM_IO_PB_21

// MASTER SPI configuratioin cs--PB14, ck--PB15, di--PB16, do--PB17
#define DF_spi_cs_config WM_IO_PB_14
#define DF_spi_ck_config WM_IO_PB_15
#define DF_spi_di_config WM_IO_PB_16
#define DF_spi_do_config WM_IO_PB_17
#define DF_spi_mode TLS_SPI_MODE_0
#define DF_spi_cs_active TLS_SPI_CS_LOW
#define DF_spi_clk 1000000
// MASTER SPI configuratioin end

// uart config begin
// TLS_UART_0 - это usb-uart
// TLS_UART_1 ... TLS_UART_5 это
#define DF_UART_SEL TLS_UART_1
#define DF_UART_BAUDRATE 115200
// else not TLS_UART_0, configure next
#define DF_UART_rx_config WM_IO_PB_07
#define DF_UART_tx_config WM_IO_PB_06
#define DF_UART_paritytype TLS_UART_PMODE_DISABLED
#define DF_UART_stopbits TLS_UART_ONE_STOPBITS
#define DF_UART_charlength TLS_UART_CHSIZE_8BIT
#define DF_UART_flow_ctrl TLS_UART_FLOW_CTRL_NONE
// uart config end

#endif /* end of MY_CONFIG_H */
