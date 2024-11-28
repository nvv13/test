add_executable(TaskMutex
        main.c
)

pico_enable_stdio_usb(TaskMutex 1)
pico_enable_stdio_uart(TaskMutex 0)

target_link_libraries(TaskMutex pico_stdlib freertos)
pico_add_extra_outputs(TaskMutex)

