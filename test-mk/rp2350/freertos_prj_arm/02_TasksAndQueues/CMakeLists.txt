add_executable(tasks_queues_blink
        main.c
)

target_link_libraries(tasks_queues_blink pico_stdlib freertos)

pico_enable_stdio_usb(tasks_queues_blink 1)
pico_enable_stdio_uart(tasks_queues_blink 0)

pico_add_extra_outputs(tasks_queues_blink)
