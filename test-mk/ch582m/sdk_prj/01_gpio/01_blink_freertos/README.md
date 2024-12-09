!!!В РАБОТЕ!!!
!!!underway!!!



исходник 
src:
 https://www.wch.cn/downloads/CH583EVT_ZIP.html 
 https://github.com/openwch/ch583.git

идея из
 https://github.com/cch9897/CH32V203-FreeRTOS-Cmake.git

собираем так


делаем директорию build
переходим в нее
и запускаем cmake..
потом make
и далее, прошить...


~~~

$ mkdir build

$ cd build

$ cmake ..

$ make

вот это еще не пробовал
$ ../tools/minichlink/minichlink -w ch582m-FreeRTOS.bin 0x08000000


~~~

Соединения
~~~
PA1 - LED

~~~