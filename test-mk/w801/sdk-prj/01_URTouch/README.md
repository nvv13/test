!!!В РАБОТЕ!!!
!!!underway!!!



project URTouch

главный источник: 
http://www.rinkydinkelectronics.com/library.php?id=92


-----------------------------


-----------------------------

board HLK-W801-KIT-V1.1 + Резистивный сенсорный экран 3.2 TFT дисплей 320X240 16bit bus

Видео


Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/3.2tft_back.jpg" alt="back side" title="back side" /></p>
фото2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/3.2tft.jpg" alt="top side" title="top side" /></p>


соединения

надо соеденить по схеме:
~~~
вид с низу, разьем слева
connect to TFT_320QDT_9341
---- ------------ ----
W801 LCD   LCD    W801
---- ------------ ----
       20  40
       NC  NC
3.3v LEDA  NC
       NC  SD_CS
PA04 REST  SD_DIN
     F_CS  SD_CLK
PA03   CS  SD_DO
PB21 DB15  T_IRQ  PA9
PB22 DB14  T_DO   PA8
PB23 DB13  NC
PB24 DB12  T_DIN  PA7
PB25 DB11  T_CS   PA6
PB26 DB10  T_CLK  PA5
PB18  DB9  DB7    PB16
PB17  DB8  DB6    PB01
3.3v   RD  DB5    PB02
PA02   WR  DB4    PB15 
PA01   RS  DB3    PB14
       NC  DB2    PB13 
 
5V    VCC  DB1    PB12
gnd   GND  DBO    PB00
        1   21
---- ------------ ----
W801  LCD  LCD    W801
---- ------------ ----



~~~



------------------------------------------------







---------------------------------------------



-----------------------------
после сборки, файл libUTFT.a 
лежит ./bin/build/w800/lib/libURTouch.a
нужно закинуть в общие библиотеки ./lib/w800/mod1

также фалик URTouch.h 
 лежит ./app/UTFT
 скоприровать в директорию ./include/mod1

это для других проектов

-----------------------------





-----------------------------
-----------------------------


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







Прочее:

  подцепляемся к платке, если интересует вывод на консоль (используеться в проекте)

$ picocom --echo -b 115200 /dev/ttyUSB0

  после подсоединения платка "висит", это из за сигнала RTS (про это есть обьяснение в статье https://www.cnblogs.com/milton/p/15621540.html)
  поэтому делаем CTRL + A + G   - это комманда picocom - RTS down, (нажимать надо достаточно быстро, (одновременно, но сначала CTRL))
  и пользуемся ...
          выход из picocom - CTRL + A + Q  (это выход без сброса порта (Skipping tty reset...))







--------------------------------

главный источник: 
## License and Credits 
This project is based on the original [URTouch library](http://www.rinkydinkelectronics.com/library.php?id=92) developed and maintained by Henning Karlsen, and licensed under [CC BY-NC-SA 3.0](http://creativecommons.org/licenses/by-nc-sa/3.0/).

--------------------------------

