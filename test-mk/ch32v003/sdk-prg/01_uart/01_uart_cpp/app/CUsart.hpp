/*


connect to
v003     ext USART
-----   ------ 
PD5(TX)   RX    - это чтобы увидеть вывод программы
PD6(RX)   TX    - это чтобы послать ответ

BaudRate = 115200

например
$ picocom --echo -b 115200 /dev/ttyACM0 -подключаемся к WCH-LinkE 


*/

#pragma once

#include <stdbool.h>

class CUsart
{
private:
  static CUsart *instance;

protected:
  CUsart (void);

public:
  static CUsart *GetInstance ();
  bool is_recive_line (void);
  u8 *line_cstr (void);
  u8 line_len (void);
};
