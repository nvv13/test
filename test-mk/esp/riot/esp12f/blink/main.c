#include <stdio.h>
#include <string.h>

#include "shell.h"
#include "thread.h"
#include "xtimer.h"
#include "periph/gpio.h"

char stack[THREAD_STACKSIZE_MAIN];

kernel_pid_t pid;
gpio_t pin13;

void *thread_handler(void *arg)
{
    (void) arg;
    puts("I'm in \"thread\" now");
    while(true)
     {
     gpio_write(pin13,1);
     xtimer_sleep(1);
     gpio_write(pin13,0);
     xtimer_sleep(1);
     puts("blink ");
     }

    return NULL;
}

int main(void)
{

    xtimer_sleep(1);
    puts("This is blink");
    //xtimer_init();	
    pin13 = GPIO_PIN(0, 2); // LILO esp12f pin2
    //pin13 = GPIO_PIN(0, 13);
    //pin13 = GPIO_PIN(0, 16);
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
