
исходник 
src:
 https://www.wch.cn/downloads/CH583EVT_ZIP.html 
 https://github.com/openwch/ch583.git



идея файла cmake из
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


  перед этим, надо удерживая кнопку "boot" - подключить к USB компа, и тогда прошивать...

$ make flash



~~~

Соединения
~~~
PB4 - LED (у меня уже есть на отладочной плате)

~~~