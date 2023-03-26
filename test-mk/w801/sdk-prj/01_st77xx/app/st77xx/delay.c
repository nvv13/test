#include "wm_osal.h"

void delay_ms(unsigned int ms)
{
u32 tick = ms / (1000/HZ);
if(ms> (tick * (1000/HZ)) )tick++;//в данном случае в большую сторону сделаем
tls_os_time_delay(tick);
};
