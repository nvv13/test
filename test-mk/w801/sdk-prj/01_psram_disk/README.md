
project psram_disk DEMO

цель, скачать jpg с интернета, и отобразить на дисплее




------------------------------------------------


board HLK-W801-KIT-V1.1 + 3.2 TFT IPS 320xRGBx480 MSP3526


Видео


Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/MSP3526_back.jpg" alt="back side" title="back side" /></p>
фото 2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/MSP3526.jpg" alt="top side" title="top side" /></p>


wiki
http://www.lcdwiki.com/3.5inch_IPS_SPI_Module_ST7796


соединения

надо соединить по схеме:
~~~


версия с использованием SDIO (60 Mhz) 

для SDIO надо соединить по схеме:
connect to MSP3526
---- ------
W801  LCD   
---- ------
      SD_CS
PA09  CTP_INT     прерывание
PA04  CTP_SDA     
PB21  CTP_RST     сброс CTP  (не должен висеть "в воздухе"!)
PA01  CTP_SCL     

      SDO(MISO)
3.3v  LED         подсветка
PB06  SCK         синхросигнал
PB07  SDI(MOSI)
PB22  LCD_RS      комманда/данные
PB21  LCD_RST     сброс
PB23  LCD_CS      выбор чипа
gnd   GND         земля
3v3   VDD         питание (3.3v - 5v)
---- ------
W801  LCD   
---- ------






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

---

~~~





--------------------------------------------------











-----------------------------


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

2) далее качаем SDK

    перемещаемся в домашнюю директорию
 
$ cd ~

    делаем директорию для проекта

$ mkdir w801

$ cd w801  

$ git clone https://github.com/leoay/wm-sdk-w80x-2023.git

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

+ещё [UTFT_Buttons library](http://www.rinkydinkelectronics.com/library.php?id=61)

--------------------------------


