// ********************************************************************
// test
// ********************************************************************

#include "cplusplus.h"
#include "CUsart.hpp"

extern "C" int
main (void)
{

  /* setup */
  CUsart *oUsart = CUsart::GetInstance ();

  /* loop */
  while (1)
    {
      if(oUsart->is_recive_line())
       {
          printf ("Recived #%d: %s\n", oUsart->line_len(), oUsart->line_cstr());
       }
    }

}
