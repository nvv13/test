
#include <stdio.h>

#include "esp32-hal-log.h"

void
ESP_LOGD (const char *ch, const char *mess)
{
  printf ("%s-%s\n", ch, mess);
};
