#include "mongoose_custom.h" 

uint64_t mg_millis(void) {
  //return xTaskGetTickCount() * portTICK_PERIOD_MS;
  return tls_os_get_time () * (1000 / HZ);
}

