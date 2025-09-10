// ********************************************************************
// src  https://count-zero.ru/2015/printf/
//      https://microsin.net/programming/avr/atmega2560-working-with-uart.html
// ********************************************************************

#define LED_PIN PB7 // build in led D13

// #define F_CPU 16000000UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#define LEN 100
static char buffer[LEN + 1];

static volatile unsigned char done;
static volatile unsigned char IDX;

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
      IDX = 0;
      done = 1;
    }
}

static void
blink13 (uint8_t count)
{
  PORTB |= (1 << LED_PIN);
  count = (count << 1);
  count--;
  for (uint8_t IT = 0; IT < count; IT++)
    {
      _delay_ms (500);
      PORTB ^= (1 << LED_PIN);
    };
};

static FILE mystdout
    = FDEV_SETUP_STREAM (uart_putchar, NULL, _FDEV_SETUP_WRITE);

int
main (void)
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

  DDRB |= (1 << LED_PIN); //  pinMode(13,OUTPUT);
  blink13 (3);            // ready indication

  stdout = &mystdout;
  int count = 0;
  for (;;)
    {
      if (done)
        {
          count++;
          PORTB |= (1 << LED_PIN);
          printf ("Recived #%d: %s\n", count, buffer);
          PORTB &= ~(1 << LED_PIN);
          done = 0;
        }
    }
  return 0;
}
