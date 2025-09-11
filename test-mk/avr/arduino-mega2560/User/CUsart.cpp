
#include <stdlib.h>
// #define F_CPU 16000000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>

#include "CUsart.hpp"

#define LEN 100
static char buffer[LEN + 1];

static volatile unsigned char done;
static volatile unsigned char IDX;
static volatile uint8_t RxLineLen = 0;

static int
uart_putchar (char c, FILE *stream)
{
  if (c == '\n')
    uart_putchar ('\r', stream);
  loop_until_bit_is_set (UCSR0A, UDRE0);
  UDR0 = c;
  return 0;
}

ISR (USART0_RX_vect)
{
  char bf = UDR0;
  buffer[IDX] = bf;
  IDX++;

  if (bf == '\r' || bf == '\n' || IDX >= LEN)
    {
      if (IDX > LEN)
        buffer[LEN] = 0;
      else
        buffer[IDX] = 0;
      IDX--;
      RxLineLen = IDX;
      IDX = 0;
      done = 1;
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdangling-pointer"

CUsart *CUsart::instance = NULL;
CUsart *
CUsart::GetInstance ()
{
  if (instance == NULL)
    {
      CUsart oCUsart = CUsart ();
      instance = &oCUsart;
    }
  return instance;
}

#pragma GCC diagnostic pop

static FILE mystdout
    = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

CUsart::CUsart (void)
{
  // USART init
  // Режим двойной скорости выключен:
  UCSR0A = 0;
  // Разрешены прерывание приема, работа приемника и передатчика:
  UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
  // Настройка размера фрейма данных 8 бит, без четности, 1 стоп-бит:
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
  // Настройка скорости 115200 bps:
  UBRR0H = 0;
  UBRR0L = 8;
  //
  IDX = 0;
  done = 0;
  sei ();

  stdout = &mystdout;
}

bool
CUsart::is_recive_line (void)
{
  if (done)
    {
      done = 0;
      return true;
    }
  return false;
}

char *
CUsart::line_cstr (void)
{
  return buffer;
}

uint8_t
CUsart::line_len (void)
{
  return RxLineLen;
}
