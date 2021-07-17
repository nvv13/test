#include <stdio.h>
#include <string.h>

#include "shell.h"
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"
#include "periph/adc.h"

char stack[THREAD_STACKSIZE_MAIN];

kernel_pid_t pid;
gpio_t pin13;

adc_t line1;
adc_res_t res1;
int32_t i_adc_result;

void *thread_handler(void *arg)
{
    (void) arg;
    puts("I'm in \"thread\" now");
    while(true)
     {
     i_adc_result=adc_sample(line1, res1);
     gpio_write(pin13,1);
     xtimer_sleep(1);
     //xtimer_usleep(i_adc_result);
     gpio_write(pin13,0);
     xtimer_sleep(1);
     //puts("blink ");
     printf("adc result = %ld\n",  i_adc_result);
     }

    return NULL;
}

int main(void)
{

    xtimer_sleep(1);
    puts("This is ADC sample");
    //xtimer_init();

/**
 * @brief   Initialize the given ADC line
 *
 * The ADC line is initialized in synchronous, blocking mode.
 *
 * @param[in] line          line to initialize
 *
 * @return                  0 on success
 * @return                  -1 on invalid ADC line
 */	
    line1=ADC_LINE(0); // pin A0
    res1=ADC_RES_12BIT;
    if(adc_init(line1)==-1)
     {
     puts("invalid ADC line");
     }
    puts("ADC init OK");

    pin13 = GPIO_PIN(PORT_C, 13);
    gpio_init(pin13,GPIO_OUT);


    /* ... */
    pid=thread_create(stack, sizeof(stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_WOUT_YIELD, // THREAD_CREATE_STACKTEST,
                  thread_handler, NULL, "thread_handler");
                  
                  
    // thread_wakeup(pid);

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
