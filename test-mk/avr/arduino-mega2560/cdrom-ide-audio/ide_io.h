#ifndef IDE_IO_H_
#define IDE_IO_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <avr/io.h>

  class ide_io
  {
  public:
    union data16
    {
      struct
      {
        uint8_t low;  // low order uint8_t
        uint8_t high; // high order uint8_t
      };
      uint16_t val; // 16 bits presentation
    };

    ide_io ();
    data16 read (uint8_t regval);
    void write (uint8_t regval, uint8_t dataLval, uint8_t dataHval);
    void reset ();

  private:
    uint8_t _control_reg;
    bool _debug_ide_rw;
  };

#ifdef __cplusplus
}
#endif

#endif
