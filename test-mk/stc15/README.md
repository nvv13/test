# stc15w408as

попалась как то мне микросхема мк stc15w408as,

STC15W408AS
~~~
======================================================================================
входов/выходов = до 26 , в зависимости от корпуса бывает в SOP16/20/28 или DIP16/20/28
                 нагрузка на вывод до 20mA, но не более 90mA на весь чип.
                 (частоты до 13.5MHz при питании 5V, и до 8MHz при питании 3.3V)
частоты        = между 5MHz и 35MHz (5.5296MHz/11.0592MHz/22.1184MHz/33.1776MHz)
                 (дрифт встроенного R/C clock, 0.6% при температурах от -20с до +65с)  
питание        = 2.5-5.5 вольт
Flash          = 8k
SRAM           = 512 байт
EEPROM         = 5к
ADC            = 8-ch, 10 bit (до 300 тысяч раз в секунду)
CCP/PCA/PWM    = 3-ch
Uart           = 1
SPI            = есть
Таймера        = 2, T0/T2 16 бит
PowerDown,
 WakeUp таймер = есть
внешних 
прервыаний     = 5  (INT0/INT1/ ~INT2/~INT3/~INT4)
Comparator     = 1 
DPTR           = 1 data pointers
Int/Low-Voltade Detector 
     Interrupt = есть
WDT            = есть
Int/High-reliable 
 Reset         = 16 level
внутренние высокостабильные 
          часы = есть 
Output cloc and Reset
    signal MCU = есть 
Encruption download
(protect code) = есть 
RS485 Control  = есть 
======================================================================================



на Али, брал вот с такой цоколёвкой(pinout):

 MK = stc15w408as        DIP20/SOP20:
   --------------------------------------------------------
 1 P1.2/ADC2/SS/ECI/CMPO                  CCP0/ADC1/P1.1 20
 2 P1.3/ADC3/MOSI                         CCP1/ADC0/P1.0 19
 3 P1.4/ADC4/MISO            CCP2_2/CCP2/TxD_2/INT3/P3.7 18
 4 P1.5/ADC5/SCLK                 CCP1_2/RxD_2/INT2/P3.6 17
 5 P1.6/ADC6/RxD_3/XTAL2/MCLKO_2      CCP0_2/T0CLKO/P3.5 16
 6 P1.7/ADC7/TxD_3/XTAL1                   ECI_2/T0/P3.4 15
 7 P5.4/RST/MCLKO/CMP-                         INT1/P3.3 14
 8 Vcc                                         INT0/P3.2 13
 9 P5.5/CMP+                                 T2/TxD/P3.1 12
10 Gnd                               T2CLKO/INT4RxD/P3.0 11
   --------------------------------------------------------

 MK = stc15w408as        DIP28/SOP28:
   ---------------------------------------------------------
 1 P2.6/CCP1_3                                P2.5/CCP0_3 28
 2 P2.7/CCP2_3                            P2.4/ECI_3/SS_2 27
 3 P1.0/ADC0/CCP1			      P2.3/MOSI_2 26
 4 P1.1/ADC1/CCP0			      P2.2/MISO_2 25
 5 P1.2/ADC2/SS/ECI/CMPO 		      P2.1/SCLK_2 24
 6 P1.3/ADC3/MOSI                         P2.0/RSTOUT_LOW 23
 7 P1.4/ADC4/MISO             CCP2_2/CCP2/TxD_2/INT3/P3.7 22
 8 P1.5/ADC5/SCLK                  CCP1_2/RxD_2/INT2/P3.6 21
 9 P1.6/ADC6/RxD_3/XTAL2/MCLKO_2       CCP0_2/T0CLKO/P3.5 20
10 P1.7/ADC7/TxD_3/XTAL1                    ECI_2/T0/P3.4 19
11 P5.4/RST/MCLKO/CMP-                          INT1/P3.3 18
12 Vcc                                          INT0/P3.2 17
13 P5.5/CMP+                                  T2/TxD/P3.1 16
14 Gnd                                T2CLKO/INT4RxD/P3.0 15
   ---------------------------------------------------------

~~~





попробовал немножко её по програмировать



enverument fedora linux 27
~~~

1.install flasher

$ git clone https://github.com/grigorig/stcgal.git

$ cd stcgal

$ python3 setup.py



2.install sdcc

[root@localhost ~]$ dnf install sdcc



3.add path

export PATH=$PATH:/usr/libexec/sdcc

к примеру, добавить в файл .bash_profile

PATH=$PATH:$HOME/.local/bin:/usr/libexec/sdcc:$HOME/bin

export PATH



4.подключаем МК к компу, например через USB-ttl платку

 USB-TxD---11(RxD)MK для stc15w408as DIP20

 USB-RxD---12(TxD)MK ...

(порт USB, прописываем в make файле)


5.очистка-сборка-заливка,
  во время заливки, 
  прога попросит вкл.выкл. питание МК - при старте он входит в режим загрузчика (кратковременно)

$ make clean
$ make
$ make flash


6.первоначально, если не прошьется, (новый МК), надо "прошить" с выставлением частоты
  через софт с сайта STCmicro.com (Windows прога) http://www.stcmicro.com/rar/stc-isp6.86.rar
  (или архив в каталоге .\tools\stc-isp-15xx-v6.89G.zip)

описание STC15-English.pdf, там надо искать STC15W401AS series MCU
 про микроконтроллеры, вида 8051/8052 , много есть в интернете, например:
 Микроконтроллеры семейства MCS-51 теория и практика
 https://elar.urfu.ru/bitstream/10995/52389/1/978-5-7996-2167-4_2017.pdf
 https://www.tutorialspoint.com/embedded_systems/index.htm
 https://www-tutorialspoint-com.translate.goog/developers_best_practices/write_to_be_reviewed.htm?_x_tr_sl=en&_x_tr_tl=ru&_x_tr_hl=ru&_x_tr_pto=wapp
и т.д.


про компилятор sdcc на сайте http://sdcc.sourceforge.net
http://sdcc.sourceforge.net/doc/sdccman.pdf
~~~





