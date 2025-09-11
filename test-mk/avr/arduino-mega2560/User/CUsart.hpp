/*


*/

#pragma once

#include <stdbool.h>
#include <stdio.h>

class CUsart
{
private:
  static CUsart *instance;

protected:
  CUsart (void);

public:
  static CUsart *GetInstance ();
  bool is_recive_line (void);
  char *line_cstr (void);
  uint8_t line_len (void);
};
