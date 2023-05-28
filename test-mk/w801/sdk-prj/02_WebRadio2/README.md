project simple WebRadio

board HLK-W801-KIT-V1.1 + VS1053 = simple WebRadio


видео:
https://youtu.be/WgBXT7bLOw8

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

3.97 TFT_397T_NT35510 800x480 16bit bus 


надо соединить по схеме:
~~~
вид с низу, разьем слева
connect to TFT_397T_NT35510
---- ------------ ----
W801 LCD      LCD W801
---- ------------ ----
PA01 RS      CS   PA03
3.3v RD      WR   PA02
PB00 DBO     RST  PA04 
PB13 DB2     DB1  PB12
PB15 DB4     DB3  PB14
PB01 DB6     DB5  PB27
PB17 DB8     DB7  PB16
PB26 DB10    DB9  PB18
PB24 DB12    DB11 PB25
PB22 DB14    DB13 PB23
     NC      DB15 PB21
3.3v VCC       BL 3.3v 
gnd  GND      VCC 3.3v 
     NC       GND gnd 
PA07 MOSI    MISO PA08
     NC       PEN PA09
PA05 CLK     T_CS PA06
---- ------------ ----
W801 LCD      LCD W801
---- ------------ ----


W801  VS1053
----- -------------
PA10  XRST
PA11  XCS
PA12  XDCS
PA13  DREQ
PB02  SCK
PB03  MISO
PB05  MOSI
5V    5V
GND   DGND
----- -------------



~~~



--------

в проекте используеться библиотека u8g2 + библиотека VS1053




--------









Сборка  (это не конкретно этого проекта, а к примеру видео https://www.youtube.com/watch?v=uMJ7SQkhtYc ):
~~~
делал по инструкциям из этого проекта https://github.com/droppingy/W801_SDK-linux
   у меня Linux Fedora 33 x86_64, думаю для всех Linux x86_64 подойдет

1) сначала, один раз, ставим утилиты для сборки c-sky (архитектура w806, w801 мк)
   благодаря пользователю droppingy это очень просто!  

$ sudo git clone https://github.com/droppingy/hlk-w80x-toolchain.git /opt/w80x-tools

   так чтобы в итоге получилось /opt/w80x-tools/bin
   это чтоб в menuconfig не переделывать, такой там путь по умолчанию
   (еще, в этом мануале https://github.com/IOsetting/wm-sdk-w806, есть ссылки как получить данный Toolchain на другие архитектуры, типа i386)

2) далее качаем SDK

    перемещаемся в домашнюю директорию
 
$ cd ~

    делаем директорию для проекта

$ mkdir w801

$ cd w801  

$ git clone https://github.com/droppingy/W801_SDK-linux.git

3) далее, собираем приложение

$ cd W801_SDK-linux

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






