add_executable(TempSense
        main.c
)

pico_enable_stdio_usb(TempSense 1)
pico_enable_stdio_uart(TempSense 0)

target_link_libraries(TempSense pico_stdlib hardware_adc freertos)
pico_add_extra_outputs(TempSense)

