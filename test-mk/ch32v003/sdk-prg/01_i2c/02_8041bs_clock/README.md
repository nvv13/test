
Платка CH32V003 или её еще просто обзывают TWen32F003 на МК CH32V003F4P6

![photo](../../../doc/TWen32F003.jpg)


Видео:

https://youtu.be/wZxzk2wER3Y

https://rutube.ru/video/7dd18879fff89e7681880fa2ce01eb89/



соединения

надо соединить по схеме:
~~~
компоненты

(1)
module ds3231 с eeprom на борту 24c32
вот такой (для примера только)
https://protosupplies.com/product/ds3231s-rtc-with-eeprom-module/

(2)
CH32V003F4P6



connect to
v003   module ds3231 
-----  ------ 
PC2    SCL   i2c clock   
PC1    SDA   i2c data   
V      VCC     
G      GND    
-----  ------ 


connect to
v003 8041BS-1 
---- ----- 
PA1  1  E
PD4  2  D
PD3  3  DP
PD2  4  C
PA2  5  G
PD0  6  Dig4
PC7  7  B
PC6  8  Dig3
PC5  9  Dig2
PC4  10 F
PC3  11 A
PC0  12 Dig1
---- ------ 

или
connect to (MODE_COMMON_ANODE)
v003 8042BS-1F 
---- ----- 
PA1  1  E
PD4  2  D
     3  DP
PD2  4  C
PA2  5  G
PD0  6  Dig4
 PC5 7  D1+D2 ANOD
PD3  8  D1+D2 CATHOD
PC7  9  B
PC6  10 Dig3
 PC5 11 Dig2
PC4  12 F
PC3  13 A
PC0  14 Dig1
---- ------ 



Только для прошивки, и установки времени в часах
connect to
v003   WCH-LinkE
-----   ------ 
PD5(TX)   RX    - это чтобы увидеть вывод программы
PD6(RX)   TX    - это чтобы послать ответ
                 $ picocom --echo -b 115200 /dev/ttyACM0 -подключаемся к WCH-LinkE 
SWD(PD1)  SWDIO
V         3V3
G         GND    
-----  ------ 


~~~


