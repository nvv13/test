# stc15w408as

попалась как то мне микросхема мк stc15w408as,

STC15W408AS
===================================================================================
входов/выходов = до 26 , в зависимости от корпуса бывает в SOP16/20/28 или DIP16/20/28
питание        = 2.5-5.5 вольт
Flash          = 8k
SRAM           = 512 байт
EEPROM         = 5к
ADC            = 8-ch, 10 bit
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
===================================================================================




попробовал немножко её по програмировать



enverument fedora linux 27

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


про данные типы микроконтроллеров, много есть в интернете, например:
 Микроконтроллеры семейства MCS-51 теория и практика
 https://elar.urfu.ru/bitstream/10995/52389/1/978-5-7996-2167-4_2017.pdf
https://www.tutorialspoint.com/embedded_systems/index.htm
https://www-tutorialspoint-com.translate.goog/developers_best_practices/write_to_be_reviewed.htm?_x_tr_sl=en&_x_tr_tl=ru&_x_tr_hl=ru&_x_tr_pto=wapp
и т.д.


про компилятор sdcc на сайте http://sdcc.sourceforge.net
http://sdcc.sourceforge.net/doc/sdccman.pdf





