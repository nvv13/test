
Платка 




соединения

надо соединить по схеме:
~~~


connect to
v003   WCH-LinkE
-----  ------ 
PD5    RX      - это чтобы увидеть вывод программы
                 $ picocom --echo -b 115200 /dev/ttyACM0 -подключаемся к WCH-LinkE и смотрим
SWD    SWDIO
V      3V3
G      GND    
-----  ------ 


connect to
v003   LED
-----  ------ 
PA1    LED + резистор 300 Ом - 1 Ком на землю
GND    GND    
-----  ------ 



