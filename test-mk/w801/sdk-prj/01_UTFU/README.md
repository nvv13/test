project UTFT

главный источник: 
http://www.rinkydinkelectronics.com/library.php?id=51


-----------------------------

board HLK-W801-KIT-V1.1 + 3.2 TFT дисплей Ultra HD 320X480 HX8367C 16bit bus

Видео
https://youtu.be/ULZDZhmtduk


Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/IMG_20230204_113125.jpg" alt="back side" title="back side" /></p>
фото2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/IMG_20230204_113136.jpg" alt="top side" title="top side" /></p>

для него тест (main.c) лежит в директории 01_UTFU/app/test/TFT32MEGA_2 

соединения

надо соединить по схеме:
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



*подготовка изображений для SD Card (инф.ниже)
# convert *.jpg -resize 480x320! -rotate 180 -define bmp:subtype=RGB565 j%03d-480x320.bmp

если используется библиотека libTJPEG.a
# convert *.jpg -resize 480x320! j%03d-480x320.jpg


~~~



------------------------------------------------


board HLK-W801-KIT-V1.1 + 0.96 TFT 80x160 дисплей st7735s SPI


Видео
https://youtu.be/RTD7CE0EDUw


Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/0.96tft-back.jpg" alt="back side" title="back side" /></p>
фото2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/0.96tft.jpg" alt="top side" title="top side" /></p>

для него тест (main.c) лежит в директории 01_UTFU/app/test/TFT00_96SP

соединения

надо соединить по схеме:
~~~
connect to TFT00_96SP SPI
---- ------
W801 LCD   
---- ------
gnd  GND  
3.3v VCC
PB15 SCL  синхросигнал
PB17 SDA  данные
PB21 RES  сброс (reset)
PB23 DC   комманда/данные
PB14 CS   выбор чипа
---  BLK  подсветка, можно через PWM, в данном случае не использовал
---- ------
W801 LCD   
---- ------



*подготовка изображений для SD Card (инф.ниже)
# convert *.jpg -resize 160x80! -rotate 180 -define bmp:subtype=RGB565 j%03d-160x80.bmp

если используется библиотека libTJPEG.a
# convert *.jpg -resize 160x80! j%03d-160x80.jpg


~~~



------------------------------------------------


board HLK-W801-KIT-V1.1 + 1.3 TFT 240x240 дисплей st7789 SPI

Видео
https://youtu.be/RTD7CE0EDUw?t=93

Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/1.3tft-back.jpg" alt="back side" title="back side" /></p>
фото2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/1.3tft.jpg" alt="top side" title="top side" /></p>

для него тест (main.c) лежит в директории 01_UTFU/app/test/TFT1_3SP

соединения

надо соединить по схеме:
~~~
connect to TFT1_3SP SPI
---- ------
W801 LCD   
---- ------
gnd  GND  
3.3v VCC
PB15 SCL  синхросигнал
PB17 SDA  данные
PB21 RES  сброс (reset)
PB23 DC   комманда/данные
---  BLK  подсветка, можно через PWM, в данном случае не использовал
---- ------
W801 LCD   
---- ------


*подготовка изображений для SD Card (инф.ниже)
# convert *.jpg -resize 240x240! -rotate 180 -define bmp:subtype=RGB565 j%03d-240x240.bmp

если используется библиотека libTJPEG.a
# convert *.jpg -resize 240x240! j%03d-240x240.jpg


~~~



------------------------------------------------


board HLK-W801-KIT-V1.1 + 1.14 TFT 135x240 дисплей st7789v SPI


Видео


Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/1.14tft-back.jpg" alt="back side" title="back side" /></p>
фото2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/1.14tft.jpg" alt="top side" title="top side" /></p>

для него тест (main.c) лежит в директории 01_UTFU/app/test/TFT01_14V89

соединения

надо соединить по схеме:
~~~
connect to TFT01_14V89 SPI
---- ------
W801 LCD   
---- ------
gnd  GND  
3.3v VCC
PB15 SCL  синхросигнал
PB17 SDA  данные
PB21 RES  сброс (reset)
PB23 DC   комманда/данные
PB14 CS   выбор чипа
---  BLK  подсветка, можно через PWM, в данном случае не использовал
---- ------
W801 LCD   
---- ------


*подготовка изображений для SD Card (инф.ниже)
# convert *.jpg -resize 240x135! -rotate 180 -define bmp:subtype=RGB565 j%03d-240x135.bmp

если используется библиотека libTJPEG.a
# convert *.jpg -resize 240x135! j%03d-240x135.jpg

~~~



------------------------------------------------


board HLK-W801-KIT-V1.1 + 1.47 TFT 172x320 дисплей st7789v3 SPI


Видео


Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/1.47tft-back.jpg" alt="back side" title="back side" /></p>
фото2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/1.47tft.jpg" alt="top side" title="top side" /></p>

для него тест (main.c) лежит в директории 01_UTFU/app/test/TFT01_47V89

соединения

надо соединить по схеме:
~~~
connect to TFT01_47V89 SPI
---- ------
W801 LCD   
---- ------
gnd  GND  
3.3v VCC
PB15 SCL  синхросигнал
PB17 SDA  данные
PB21 RES  сброс (reset)
PB23 DC   комманда/данные
PB14 CS   выбор чипа
---  BLK  подсветка, можно через PWM, в данном случае не использовал
---- ------
W801 LCD   
---- ------



*подготовка изображений для SD Card (инф.ниже)
# convert *.jpg -resize 320x172! -rotate 180 -define bmp:subtype=RGB565 j%03d-320x172.bmp

если используется библиотека libTJPEG.a
# convert *.jpg -resize 320x172! j%03d-320x172.jpg

~~~



------------------------------------------------




------------------------------------------------


board HLK-W801-KIT-V1.1 + 1.69 TFT 240x280 дисплей st7789v2 SPI


Видео


Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/1.69tft-back.jpg" alt="back side" title="back side" /></p>
фото2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/1.69tft.jpg" alt="top side" title="top side" /></p>

для него тест (main.c) лежит в директории 01_UTFU/app/test/TFT01_69V89

соединения

надо соединить по схеме:
~~~
connect to TFT01_69V89 SPI
---- ------
W801 LCD   
---- ------
gnd  GND  
3.3v VCC
PB15 SCL  синхросигнал
PB17 SDA  данные
PB21 RES  сброс (reset)
PB23 DC   комманда/данные
PB14 CS   выбор чипа
---  BLK  подсветка, можно через PWM, в данном случае не использовал
---- ------
W801 LCD   
---- ------



*подготовка изображений для SD Card (инф.ниже)
# convert *.jpg -resize 280x240! -rotate 180 -define bmp:subtype=RGB565 j%03d-280x240.bmp

если используется библиотека libTJPEG.a
# convert *.jpg -resize 280x240! j%03d-280x240.jpg

~~~



------------------------------------------------


-----------------------------

board HLK-W801-KIT-V1.1 + 3.2 TFT дисплей 320X240 16bit bus

Видео
https://youtu.be/Jpsm4nAKuIg

Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/3.2tft_back.jpg" alt="back side" title="back side" /></p>
фото2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/3.2tft.jpg" alt="top side" title="top side" /></p>

для него тест (main.c) лежит в директории 01_UTFU/app/test/TFT_320QDT_9341

соединения

надо соединить по схеме:
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
PB21 DB15  T_IRQ
PB22 DB14  T_DO
PB23 DB13  NC
PB24 DB12  T_DIN
PB25 DB11  T_CS
PB26 DB10  T_CLK
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



*подготовка изображений для SD Card (инф.ниже)
# convert *.jpg -resize 320x240! -rotate 180 -define bmp:subtype=RGB565 j%03d-320x240.bmp

если используется библиотека libTJPEG.a
# convert *.jpg -resize 320x240! j%03d-320x240.jpg


~~~



------------------------------------------------


board HLK-W801-KIT-V1.1 + 2.4 TFT SPI 240x320 V1.3


Видео


Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/TFT2_4SP_9341_back.jpg" alt="back side" title="back side" /></p>
фото 2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/TFT2_4SP_9341.jpg" alt="top side" title="top side" /></p>


для него тест (main.c) лежит в директории 01_UTFU/app/test/TFT2_4SP_9341

соединения

надо соединить по схеме:
~~~
connect to TFT2_4SP_9341
---- ------
W801 LCD   
---- ------
3.3v VCC
gnd  GND  
PB21 RESET сброс
PB14 CS    выбор чипа
PB23 DC    комманда/данные
PB17 SDI   данные
PB15 SCK   синхросигнал
3.3v LED  
---- ------
W801 LCD   
---- ------



*подготовка изображений для SD Card (инф.ниже)
# convert *.jpg -resize 320x240! -rotate 180 -define bmp:subtype=RGB565 j%03d-320x240.bmp

если используется библиотека libTJPEG.a
# convert *.jpg -resize 320x240! j%03d-320x240.jpg

~~~






------------------------------------------------


board HLK-W801-KIT-V1.1 + 2.0 TFT SPI 240x320 


Видео
https://youtu.be/fxX3JLcDdj8

Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/TFT02_0V89_back.jpg" alt="back side" title="back side" /></p>
фото 2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/TFT02_0V89.jpg" alt="top side" title="top side" /></p>


для него тест (main.c) лежит в директории 01_UTFU/app/test/TFT02_0V89

соединения

надо соединить по схеме:
~~~
connect to TFT02_0V89 
---- ------
W801 LCD   
---- ------
gnd  GND  
3.3v VCC
PB15 SCL  синхросигнал
PB17 SDA  данные
PB21 RES  сброс (reset)
PB23 DC   команда/данные
PB14 CS   выбор чипа
---- ------
W801 LCD   
---- ------



*подготовка изображений для SD Card (инф.ниже)
# convert *.jpg -resize 320x240! -rotate 180 -define bmp:subtype=RGB565 j%03d-320x240.bmp

если используется библиотека libTJPEG.a
# convert *.jpg -resize 320x240! j%03d-320x240.jpg

~~~



------------------------------------------------


Если нужно, в тесте дисплея Вывод картинок,то надо подключить SD Card
на SD Card должны быть файлы изображения типа .BMP, не сжатыйе,
 подготовленный в нужном разрешении с палитрой 565
в названии (en) должно быть разрешения текущего подключенного дисплея,
например "mkar1-320x172.bmp"

Как подготовить файлы, способы:
1)с помощью конверторов идущих с библиотекой UTFT
   http://www.rinkydinkelectronics.com/library.php?id=51
 в raw формат

2) для Linux, это можно сделать с помощью утилиты convert, входящую в состав
ImageMagick. [Руководство пользователя ImageMagickv. 7.1.0](https://coollib.net/b/558566-ivan-georgievich-titarenko-rukovodstvo-polzovatelya-imagemagick-v-710)
 или
 https://www.altlinux.org/ImageMagick_-_%D0%BE%D0%B1%D1%80%D0%B0%D0%B1%D0%BE%D1%82%D0%BA%D0%B0_%D0%B8%D0%B7%D0%BE%D0%B1%D1%80%D0%B0%D0%B6%D0%B5%D0%BD%D0%B8%D0%B9

например, конвертировать все изображения png (или jpg ниже), текущей
директории, в bmp формат размера 172 на 320 без сохранения соотношения сторон
для формата цвета пикселя RGB565 (такой нужен для TFT дисплея)

# convert *.jpg -resize 320x172! -rotate 180 -define bmp:subtype=RGB565 j%03d-320x172.bmp

если используется библиотека libTJPEG.a
# convert *.jpg -resize 320x172! j%03d-320x172.jpg

~~~
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

~~~

---------------------------------------------



-----------------------------
после сборки, файл libUTFT.a 
лежит ./bin/build/w800/lib/libUTFT.a
нужно закинуть в общие библиотеки ./lib/w800/mod1
(и ещё libTJPEG.a)

также фалик UTFT.h 
 лежит ./app/UTFT
 скоприровать в директорию ./include/mod1
(и ещё u_jpeg.h лежит в ./app/UTFTjpeg/)

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


исходники библиотеки, взяты отсюда: 
https://wiki.iarduino.ru/page/rabota-s-cvetnym-graficheskim-displeem/ 

добавлен ILI9225B из: 
https://github.com/gnulabis/UTFT-ESP.git 

добавлены русские буквы из: 
https://github.com/twsdwf/UTFT-rus.git

главный источник: 
## License and Credits 
This project is based on the original [UTFT library](http://www.rinkydinkelectronics.com/library.php?id=51) developed and maintained by Henning Karlsen, and licensed under [CC BY-NC-SA 3.0](http://creativecommons.org/licenses/by-nc-sa/3.0/).


исходники для jpeg библиотеки, взяты отсюда:
https://gitee.com/openLuat/LuatOS.git

--------------------------------


