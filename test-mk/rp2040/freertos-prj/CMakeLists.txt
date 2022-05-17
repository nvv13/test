cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(Pico-FreeRTOS)

pico_sdk_init()

add_subdirectory(freertos)

add_subdirectory(01_TaskGPIO)
add_subdirectory(02_TasksAndQueues)
add_subdirectory(03_TaskScheduler)
add_subdirectory(04_TaskMutex)
add_subdirectory(05_Semaphores)
add_subdirectory(06_TempSense)
add_subdirectory(07_quadrature_encoder)


