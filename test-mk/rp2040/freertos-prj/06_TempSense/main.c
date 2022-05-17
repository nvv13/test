#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"


void temperature_task(){

    // Configure ADC
    adc_init(); // инициализируйте АЦП 
    adc_set_temp_sensor_enabled(true); // включите датчик температуры
    adc_select_input(4); // устанавливаем вход АЦП на 4, что соответствует 5-му каналу АЦП.

    while(true){
        uint16_t raw = adc_read();
        const float conversion_factor = 3.3f / (1<<12);
        float result = raw * conversion_factor;
        float temp = 27 - (result -0.706)/0.001721;
        printf("Temp = %f C\n", temp);
        vTaskDelay(1000);
    }
}


int main()
{
    stdio_init_all();

    xTaskCreate(temperature_task, "temperature_task", 256, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1){};
}
