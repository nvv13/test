!!!В РАБОТЕ!!!
!!!underway!!!


project UTFT

board HLK-W801-KIT-V1.1 + 3.2 TFT дисплей Ultra HD 320X480 HX8367C

Видео
https://youtu.be/ULZDZhmtduk


Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/IMG_20230204_113125.jpg" alt="back side" title="back side" /></p>
фото2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/IMG_20230204_113136.jpg" alt="top side" title="top side" /></p>



соединения

надо соеденить по схеме:
~~~
на LCD если смотреть сверху на экран, и разьем с правой стороны
то, соединения такие
connect to TFT32MEGA_2 
---- --------- ----
W801 LCD   LCD W801
---- --------- ----
5v    5v   5v	
PB17 VH0   VH1 PB18
PB26 VH2   VH3 PB25 
PB24 VH4   VH5 PB23 
PB22 VH6   VH7 PB21 
PB16 VL7   VL6 PB01
PB02 VL5   VL4 PB15 
PB14 VL3   VL2 PB13 
PB12 VL1   VL0 PB00
PA01 RS	    WR PA02	
PA03 CS    RST PA04
     -*-   -*-
     -*-   RD
     -*-   -*-
     -*-   -*-
 MISO SD   SD MOSI
  SCK SD   SD CS
gnd  GND   GND
---- --------- ----
W801 LCD   LCD W801
---- --------- ----



~~~







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










исходники библиотеки, взяты отсюда: 
https://wiki.iarduino.ru/page/rabota-s-cvetnym-graficheskim-displeem/ 

добавлен ILI9225B из: 
https://github.com/gnulabis/UTFT-ESP.git 

главный источник: 
## License and Credits 
This project is based on the original [UTFT library](http://www.rinkydinkelectronics.com/library.php?id=51) developed and maintained by Henning Karlsen, and licensed under [CC BY-NC-SA 3.0](http://creativecommons.org/licenses/by-nc-sa/3.0/).





