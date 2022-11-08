#include "wm_type_def.h"

//#include "wm_watchdog.h"

#include <string.h>
#include "wm_include.h"
#include "wm_demo.h"
#include "wm_pmu.h"


static void pmu_timer1_irq(u8 *arg)
{
    tls_pmu_timer1_stop();
    printf("pmu timer1 irq\n");
}


int pmu_timer1_demo(u8 enterStandby)
{
	tls_pmu_clk_select(0);/*0:select 32K RC osc, 1: select 40M divide clock*/
    tls_pmu_timer1_isr_register((tls_pmu_irq_callback)pmu_timer1_irq, NULL);
    tls_pmu_timer1_start(5000);
    printf("pmu timer1 test start\n");

    if(enterStandby)
    {
        printf("pmu will standby\n");
        tls_pmu_standby_start();/*If you want to verify sleep function, using function tls_pmu_sleep_start()*/
        printf("pmu enter standby\n");
    }
    return 	WM_SUCCESS;
}



void
UserMain (void)
{

//  tls_watchdog_init (12 * 1000
//                     * 1000); // u32 usec около 6 сек --около 1-2 минуты


}
