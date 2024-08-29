#!/usr/bin/env python
import serial, sys
serialPort = '/dev/ttyACM0'#sys.argv[1]
ser = serial.Serial(
    port=serialPort,
    baudrate=1200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
    )
ser.isOpen()
ser.close() # always close port

#sleep 2
#avrdude -v -cavr109 -patmega32u4 -P/dev/ttyACM0 -b57600 -D -Uflash:w:/home/nvv/test/test-tm1637/step01/bin/arduino-leonardo/arduino_hello-world.hex:i