
Платка CH32V003 или её еще просто обзывают TWen32F003 на МК CH32V003F4P6

![photo](../../../doc/TWen32F003.jpg)



соединения

надо соединить по схеме:
~~~


connect to
v003   WCH-LinkE
-----  ------ 
PD5       RX      - это чтобы увидеть вывод программы
                 $ picocom --echo -b 115200 /dev/ttyACM0 -подключаемся к WCH-LinkE и смотрим
SWD(PD1)  SWDIO
V         3V3
G         GND    
-----  ------ 


connect to
v003   LED
-----  ------ 
PA1    LED + резистор 300 Ом - 1 Ком на землю
GND    GND    
-----  ------ 



