# HLK W801 on Linux

Board : **HLK-W801-KIT-V1.0** (add a capacitor (**4.7uF**) between *GND* and *RESET*)

Toolchain: [**csky-elfabiv2-tools-x86_64-minilibc-20210423**](https://github.com/droppingy/hlk-w80x-toolchain)

Serial debug tool: [**picocom**](https://github.com/npat-efault/picocom) 

# Usage
1. make menuconfig
    - set download port
    - set toolchain path
2. make flash
3. picocom -b 115200 /dev/ttyUSB0
    - set **RTS** manually (CTRL + A + G)

# Ref
https://www.cnblogs.com/milton/p/15621540.html

