#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "semphr.h"

SemaphoreHandle_t count;

void led_task()
{   
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
         /*Посмотрим, сможем ли мы получить семафор. Если семафор не доступен, подождите 10 тиков, чтобы увидеть, станет ли он свободным.*/
        if(xSemaphoreTake(count, (TickType_t) 10) == pdTRUE){   // ( SemaphoreHandle_t xSemaphore,  TickType_t xTicksToWait );
            gpio_put(LED_PIN, 1);
            vTaskDelay(100);
        }
        else{
        /* Мы не смогли получить семафор и поэтому не можем получить доступ общий ресурс безопасно */
            gpio_put(LED_PIN, 0);
            vTaskDelay(1);
        }
    }
}

void button_task(){
    gpio_init(20);
    gpio_set_dir(20, GPIO_IN);

    while(true){
        if(gpio_get(20) != 0){
            /* Мы закончили доступ к общему ресурсу. Выпустить семафор. */
            xSemaphoreGive(count); // Macro to release a semaphore
            vTaskDelay(20);
        }
        else{
            vTaskDelay(1);
        }
    }
}

int main()
{
    stdio_init_all();

    count = xSemaphoreCreateCounting(5,0); // (UBaseType_t uxMaxCount, UBaseType_t uxInitialCount)

    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    xTaskCreate(button_task, "Button_Task", 256, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1){};
}
