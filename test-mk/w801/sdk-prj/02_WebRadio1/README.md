project simple WebRadio

board HLK-W801-KIT-V1.1 + VS1053 = simple WebRadio


видео:
https://youtu.be/WgBXT7bLOw8
https://rutube.ru/video/9255b7d30a5b1dd56414f1e69163a5d5/?r=wd


------
Позже (в видео нет), добавил переход в Меню1, Меню2 и обратно, по двойному клику
~~~
В Меню1:
(0) можно сохранить текущий уровень громкости (при рестарте уст.)
(1) загрузить ранее сохр станцию, позиция 0. (2) Сохранить текущую станцию, позиция 0.
(3) загрузить ранее сохр станцию, позиция 1. (4) Сохранить текущую станцию, позиция 1.
....
    и так до 44 станций                   
~~~
~~~
В Меню2:
Выбор из 10 станций последнего поиска...
~~~

------

соединения

дисплей

Светодиодный модуль OLED 1,3 дюйма, синий I2C, 128X64, 1,3 дюйма, контроллер sh1106

или

дисплей OLED 128x64 2.4 дюйма, 2.42OLED-IIC VER:1.1, I2C, контроллер ssd1309


надо соединить по схеме:
~~~

connect to
DISPLAY   w801
----- -------------
GND       GND
VCC       3.3v 
scl       PA01
sda       PA04
----- -------------


W801(1)  psram   LY68L6400* cfg(0)
------ --------- -------    -----
PA15   SCLK      (6)        PB00
PB27   CE#       (1)        PB01
PB02   SI/SIO[0] (5)
PB03   SO/SIO[1] (2)
PB04*     SIO[2] (3)
PB05      SIO[3] (7)
3.3V   Vcc       (8)
GND    Vss       (4)
------ --------- -------
* вывод PB04 на макетке board HLK-W801-KIT-V1.1 ошибочно назван PB24 в месте рядом с разьемом type-c
* esp-psram64h


W801  VS1053
----- -------------
PB17  XRST
PB21  XCS
PB22  XDCS
PB18  DREQ
PB24  SCK
PB16  MISO //PB03
PB26  MOSI
5V    5V
GND   DGND
----- -------------



Регулятор Громкости + кнопка(выбор), Энкодер KY-040 или Cap EC11
connect to
W801   KY-040 
GPIO   PIN    Cap EC11 PIN
-----  ------ -------------------------------------------
GND    GND    GND
+3.3   +      +
PA11   KEY    SW
PA12   DT     S1
PA13   CLK    S2
-----  ----- ---------------------------------------------
если на Энкодере нет RC цепочки, то надо ее добавть!
к выводам DT и CLK подключаемся через резистор 10Ком, потом конденсаторы на землю ~100пФ и это уже к W801

W801   Кнопка - переключение экранов меню
GPIO   PIN    - обязательон добавить RC цепочку
-----  ------
PA14   PIN
+3.3   + 
GND    GND
-----  ------


~~~



--------

в проекте используеться библиотека u8g2 + библиотека VS1053




--------









Сборка  (это не конкретно этого проекта, а к примеру видео https://www.youtube.com/watch?v=uMJ7SQkhtYc 
            https://rutube.ru/video/664a1f911677ecb82c431247c4a5b3b7/?r=wd ):
~~~
делал по инструкциям из этого проекта https://github.com/droppingy/W801_SDK-linux
   у меня Linux Fedora 33 x86_64, думаю для всех Linux x86_64 подойдет

1) сначала, один раз, ставим утилиты для сборки c-sky (архитектура w806, w801 мк)
   благодаря пользователю droppingy это очень просто!  

$ sudo git clone https://github.com/droppingy/hlk-w80x-toolchain.git /opt/w80x-tools

   так чтобы в итоге получилось /opt/w80x-tools/bin
   это чтоб в menuconfig не переделывать, такой там путь по умолчанию
   (еще, в этом мануале https://github.com/IOsetting/wm-sdk-w806, есть ссылки как получить данный Toolchain на другие архитектуры, типа i386)

2) далее качаем-распаковываем SDK

    перемещаемся в домашнюю директорию
 
$ cd ~

    делаем директорию для проекта

$ mkdir w801

$ cd w801  

    качаем

$ wget https://orangepi.nvv13.keenetic.link/nextcloud/index.php/s/HPkiXrT6TpeRzz3/download/W801_SDK-linux.tar.gz

    распаковка

$ tar xvf W801_SDK-linux.tar.gz


3) далее, собираем приложение

$ cd wm-sdk-w80x-2023/

     если тут запустить make, то собирается демо приложение которое идет вместе с SDK, об этом я писал кратко в https://github.com/nvv13/test/tree/main/test-mk/w801 
     но мы хотим собрать другой проект
     для этого удаляем, лишние директории, 
      где app demo - директории с демо приложением
          bin - директория где в результате будет собранное приложение, создастся заново в процессе сборки

$ rm -R app demo bin

     делаем директорию для приложения

$ mkdir app

     и, в данном случае, качаем файлики приложения, 
     для примера, возьмем отсюда https://github.com/nvv13/test/tree/main/test-mk/w801/sdk-prj/02_task_blink
     но, принцип тот же самый и для других...(если нужно много файлов, то можно воспользоваться утилитой https://downgit.github.io/)

$ cd app

$ wget https://github.com/nvv13/test/raw/main/test-mk/w801/sdk-prj/02_task_blink/app/Makefile

$ wget https://github.com/nvv13/test/raw/main/test-mk/w801/sdk-prj/02_task_blink/app/main.c

     возвращаемся в директорию SDK
$ cd ..

     заменяем головной wget Makefile на наш (удалено обращение к директории demo)

$ rm Makefile

$ wget https://github.com/nvv13/test/raw/main/test-mk/w801/sdk-prj/02_task_blink/Makefile


     перед сборкой, очистить проект, иногда надо
$ make distclean


    сборка
$ make 


    перед прошивкой, стереть флэш в микроконтроллере, иногда надо, !когда, допустим был проект с wifi, а стал без него!
$ make erase

    записать прошивку
$ make flash

    Всё!



~~~





  далее подцепляемся к платке, если интересует вывод на консоль

$ picocom --echo -b 115200 /dev/ttyUSB0

  после подсоединения платка "висит", это из за сигнала RTS (про это есть обьяснение в статье https://www.cnblogs.com/milton/p/15621540.html)
  поэтому делаем CTRL + A + G   - это комманда picocom - RTS down, (нажимать надо достаточно быстро, (одновременно, но сначала CTRL))
  и пользуемся ...
          выход из picocom - CTRL + A + Q  (это выход без сброса порта (Skipping tty reset...))


------

PS:
исходники u8g2, взяты с сайта https://github.com/olikraus/u8g2

пример адаптации взят из https://github.com/RIOT-OS/RIOT, (хотя можно было взять из LuatOS, к примеру)

библиотека VS1053 источник:
https://github.com/baldram/ESP_VS1053_Library.git
https://github.com/adafruit/Adafruit_VS1053_Library.git

------




