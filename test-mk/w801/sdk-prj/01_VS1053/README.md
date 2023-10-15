project VS1053


board HLK-W801-KIT-V1.1 + VS1053 + OLED + SD Card + Button


Видео
https://youtu.be/942BgG0cVuE


~~~
проигывает файлы(проверенно) .mp3, .flac, .ogg, .m4a
для .m4a нужно их "оптимизировать" 27 страница даташита:

Note: To be able to play the .3gp, .3g2, .mp4 and .m4a files, the mdat atom must be the
last atom in the MP4 file. Because VS1053b receives all data as a stream, all metadata must
be available before the music data is received. Several MP4 file formatters do not satisfy this
requirement and some kind of conversion is required. This is also why the streamable ADTS
format is recommended.

Programs exist that optimize the .mp4 and .m4a into so-called streamable format that has the
mdat atom last in the file, and thus suitable for web servers’ audio streaming. You can use this
kind of tool to process files for VS1053b too. For example mp4creator -optimize file.mp4 .
~~~



библиотека VS1053 источник:
https://github.com/baldram/ESP_VS1053_Library.git
https://github.com/adafruit/Adafruit_VS1053_Library.git





соединения

надо соединить по схеме:
~~~


W801  VS1053
----- -------------
PB17  XRST
PB21  XCS
PB22  XDCS
PB18  DREQ
PB24  SCK
PB03  MISO
PB26  MOSI
5V    5V
GND   DGND
----- -------------



Энкодер KY-040 или Cap EC11
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




sdio host
Подключаем SD Card по схеме
W801	Pin #	SD 4-bit Mode	Описание
-----  -------------------------------------------------
PB10	9	DAT[2]		Data Line 2 /Read Wait
 PB11	1	CD/DAT[3]	Data Line 3
 PB07	2	CMD		Command Line
 Gnd	3	VSS1		Ground
 3.3v	4	VDD		Supply Voltage
 PB06	5	CLK		Clock
 Gnd	6	Vss2		Ground
 PB08	7	DAT[0]		Data Line 0
 PB09	8	DAT[1]		Data Line 1 / Interrupt
-----  -------------------------------------------------


SD Card 
Вид со стороны контактов (с низу)
  ---------
 |12345678|
|9        |
|         |
| SD Card |
|         |
-----------

рядом с считывателем
Контакты 1,2,7,8,9 - подключаем через резисторы, примерно ~10кОм...100кОм к +3.3v контакт 4
К контактам Gnd(3,6) и +3.3v(4) - подключам конденсаторы, керамический ~0,047мКф и электролит ~10мКф от помех...






Светодиодный модуль OLED 1,3 дюйма, синий I2C, 128X64, 1,3 дюйма, контроллер sh1106
     4 pin
надо соединить по схеме:
connect to
DISPLAY     w801
-----  -------------------------------------------------
1 GND       GND
2 VCC       5v 
3 scl       PA01
4 sda       PA04
-----  -------------------------------------------------


~~~




----



в проекте используеться библиотека u8g2
~~~

ее сборка https://github.com/nvv13/test/tree/main/test-mk/w801/sdk-prj/01_u8g2_st7920_spi
после сборки, сама библиотека libu8g2.a 
 разместить в директорию 
.\lib\w800\mod1\
и заголовочные файлы u8g2.h u8x8.h u8x8_riotos.h
 разместить директорию
.\include\mod1\

их еще можно взять из 
https://github.com/nvv13/test/tree/main/test-mk/w801/W801_SDK-linux/lib/w800/mod1
https://github.com/nvv13/test/tree/main/test-mk/w801/W801_SDK-linux/include/mod1


в Makefile директории . этого проекта
секция LINKLIB добавлено
    $(TOP_DIR)/lib/$(CONFIG_ARCH_TYPE)/mod1/libu8g2$(LIB_EXT)  \

~~~


----


в SDK используеться FatFs 

 это по моему, сайт автора http://elm-chan.org/fsw/ff/00index_e.html
 и там есть подробное описание FatFs 

 тут например про её использование - настройку и примеры кода
 http://elm-chan.org/fsw/ff/doc/appnote.html


-------


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





----

PS: 
еще информации про VS1053:

https://imax9.narod.ru/publs/F407les05.html

https://github.com/baldram/ESP_VS1053_Library

  дискурсия про ring buffer
https://github.com/baldram/ESP_VS1053_Library/issues/47

https://github.com/nopnop2002/esp-idf-vs1053

PS 2: 
https://www.opennet.ru/base/sys/radio_protocols.txt.html
https://gist.github.com/niko/2a1d7b2d109ebe7f7ca2f860c3505ef0
https://cast.readme.io/docs/icy
https://github.com/schreibfaul1/ESP32-audioI2S.git

PS 3: 
 пример для изучения
$ curl -i -H 'Icy-Metadata: 1' --output 1.bin http://stream-160.zeno.fm/62twtv23sfeuv?zs=zPF1ujlnT0uSbEe2nOK3Pw









