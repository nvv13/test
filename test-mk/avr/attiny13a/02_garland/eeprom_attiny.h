#ifndef EEPROM_ATINY_H_
#define EEPROM_ATINY_H_

#include <avr/eeprom.h>

static unsigned char
read_from_internal_eeprom (unsigned int addr)
{
  while (EECR & (1 << EEPE))
    ;                  // wait for write to finish проверка готовности EEPROM
  EEAR = addr;         // load the address to read from регистр адреса
  EECR |= (1 << EERE); // set the read flag чтение EEPROM
  // the clock is stopped for 4 cyles
  EEAR = 0;    // leave address at 0 so if power down corrupts location 0
  return EEDR; // вывод значения
}

static unsigned char
write_internal_eeprom (unsigned int addr, char data)
{
  char cnt;
  for (cnt = 0; cnt < 10; cnt++)
    {
      if (data
          == (read_from_internal_eeprom (
              addr))) // will check in the read routine for write flag being
                      // cleared
        break;
      EEAR = addr; // load up address регистр адреса
      EEDR = data; // load up data  регистр данных
      // cli();//disable interrupts
      EECR |= (1 << EEMPE); // enable write Разрешение записи в EEPROM
      EECR |= (1 << EEPE);  // start write Запись в EEPROM
      // sei(); //enable interrupts
    }
  return cnt;
}

#endif