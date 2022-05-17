add_executable(Semaphores
        main.c
)

pico_enable_stdio_usb(Semaphores 1)
pico_enable_stdio_uart(Semaphores 0)

target_link_libraries(Semaphores pico_stdlib freertos)
pico_add_extra_outputs(Semaphores)

