
Платка 




соединения

надо соединить по схеме:
~~~


connect to
v203   WCH-LinkE
-----  ------ 
PD5    RX      - это чтобы увидеть вывод программы
                 $ picocom --echo -b 115200 /dev/ttyACM0 -подключаемся к WCH-LinkE и смотрим
PA13   swdio
PA14   swclk
NRST   NRST
3.3    3V3
GND    GND    
-----  ------ 


connect to
v203   LED
-----  ------ 
PA1    LED + резистор 300 Ом - 1 Ком на землю
GND    GND    
-----  ------ 



