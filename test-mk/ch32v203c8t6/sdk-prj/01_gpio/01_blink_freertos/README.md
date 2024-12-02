исходник 
src:
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

$ ../tools/minichlink/minichlink -w ch32v203-FreeRTOS.bin 0x08000000


~~~