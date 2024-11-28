add_executable(usb_serial
            usb_serial.c
            )

# pull in common dependencies
target_link_libraries(usb_serial pico_stdlib)

# enable usb output, disable uart output
pico_enable_stdio_usb(usb_serial 1)
pico_enable_stdio_uart(usb_serial 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(usb_serial)


