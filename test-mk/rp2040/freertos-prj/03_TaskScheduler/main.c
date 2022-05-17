#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include "pico/stdlib.h"


void task1(void *pvParameters)
{   
    while (true) {
        printf("Task 1 is currently running\n");
        for(int i = 0; i<20000000; i++){};
        //vTaskDelay(100);
    }
}

void task2(void *pvParameters)
{   
    while (true) {
        printf("Task 2 is currently running\n");
        for(int i = 0; i<20000000; i++){};
        //vTaskDelay(100);
    }
}

int main()
{
    stdio_init_all();

    xTaskCreate(task1, "Task 1", 256, NULL, 1, NULL);
    xTaskCreate(task2, "Task 2", 256, NULL, 2, NULL);
    vTaskStartScheduler();

    while(1){};
}
