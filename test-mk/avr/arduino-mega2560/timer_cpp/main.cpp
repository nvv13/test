// ********************************************************************
// test
// ********************************************************************

#include <util/delay.h>

#include "cplusplus.h"
#include "CUsart.hpp"
#include "SystemTime.h"

extern "C" int
main (void)
{
  /* setup */
  CUsart::GetInstance ();
  printf ("Start sysTime.Millis = %ld\n",sysTime.Millis ());

  /* loop */
  while (1)
    {
     uint32_t t1=sysTime.Millis ();
     _delay_ms (10);
     uint32_t t2=sysTime.Millis ();
     printf ("_delay_ms (10) sysTime.Millis  %ld - %ld = %ld\n",t2,t1,t2-t1);

     t1=sysTime.Millis ();
     _delay_ms (100);
     t2=sysTime.Millis ();
     printf ("_delay_ms (100) sysTime.Millis  %ld - %ld = %ld\n",t2,t1,t2-t1);

     t1=sysTime.Millis ();
     _delay_ms (1000);
     t2=sysTime.Millis ();
     printf ("_delay_ms (1000) sysTime.Millis  %ld - %ld = %ld\n",t2,t1,t2-t1);
    }

}
