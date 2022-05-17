#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "semphr.h"

static SemaphoreHandle_t mutex;

void task1(void *pvParameters)
{   
    char ch = '1';
    while (true) {
        if(xSemaphoreTake(mutex, 0) == pdTRUE){
            for(int i = 1; i < 10; i++){
                putchar(ch);
            }
            puts("");
            xSemaphoreGive(mutex);
        }
        
    }
}

void task2(void *pvParameters)
{   
    char ch = '2';
    while (true) {
        if(xSemaphoreTake(mutex, 0) == pdTRUE){
            for(int i = 1; i < 10; i++){
                putchar(ch);
            }
            puts("");
            xSemaphoreGive(mutex);
        }
        
    }
}


int main()
{
    stdio_init_all();

    mutex = xSemaphoreCreateMutex();

    xTaskCreate(task1, "Task 1", 256, NULL, 1, NULL);
    xTaskCreate(task2, "Task 2", 256, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1){};
}
