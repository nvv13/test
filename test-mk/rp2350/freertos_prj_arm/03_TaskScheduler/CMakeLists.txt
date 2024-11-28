add_executable(TaskScheduler
        main.c
)

pico_enable_stdio_usb(TaskScheduler 1)
pico_enable_stdio_uart(TaskScheduler 0)

target_link_libraries(TaskScheduler pico_stdlib freertos)
pico_add_extra_outputs(TaskScheduler)