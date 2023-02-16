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




# original source:
https://gitee.com/iosetting/wm-sdk-w80x.git
Apache License, Version 2.0, January 2004



-------

# next source:
https://github.com/droppingy/W801_SDK-linux.git

-------

02-2023 SDK обновил из:
https://github.com/leoay/wm-sdk-w80x-2023.git
и добавил перевод изменений, w801_pinout, из
https://github.com/Gusev-Roman/wm-sdk-w80x-2023.git
# Ref
http://we.easyelectronics.ru/part/w801-zhk-ekranchik-i-lozhechka-degtya.html

-------



