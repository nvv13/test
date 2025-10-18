# STC15W204S


[STC15W204S](../stc15w408as/jpg/STC15-English.pdf)
~~~
======================================================================================
входов/выходов  = до 14/6 , в зависимости от корпуса бывает в SOP16/8 или DIP8/16
                 нагрузка на вывод до 20mA, но не более 90mA на весь чип.
                 (частоты до 13.5MHz при питании 5V, и до 8MHz при питании 3.3V)
частоты         = между 5MHz и 35MHz (5.5296MHz/11.0592MHz/22.1184MHz/33.1776MHz)
                 (дрифт встроенного R/C clock, 0.6% при температурах от -20с до +65с)  
питание         = 2.4-5.5 вольт
Flash           = 4k
SRAM            = 128*2 = 256 байт
EEPROM          = 1к
Uart            = 1
Таймера         = 2, T0/T2 16 бит
PowerDown,
  WakeUp таймер = есть
внешних 
 прервыаний     = 5  (INT0/INT1/ ~INT2/~INT3/~INT4)
Comparator      = 1 
DPTR            = 1 data pointers
Int/Low-Voltade Detector 
     Interrupt  = есть
WDT             = есть
Int/High-reliable 
 Reset          = 16 level
внутренние высокостабильные 
          часы = есть 
Output clock and Reset
    signal MCU = есть 
Encruption download
 (protect code) = есть 
RS485 Control   = есть 
======================================================================================



на Али, брал вот с такой цоколёвкой(pinout):

 Pin Configurations of STC15W204S    DIP8/SOP8:
--------------------------------------------------------
CMP-/MCLKO/RST/P5.4 1    8 P3.3/INT1
		Vcc 2    7 P3.2/INT0
	  CMP+/P5.5 3    6 P3.1/TxD/T2
		Gnd 4    5 P3.0/RxD/INT4/T2CLKO
--------------------------------------------------------


бывают ещё такие
 Pin Configurations of STC15W204S    DIP16/SOP16:
--------------------------------------------------------
	T0/CMP0/P1.2 1   16 P1.1    
	        P1.3 2   15 P1.0/RSTOUT_LOW
	 T0CLKO/P1.4 3   14 P3.7/INT3/TxD_2
	        P1.5 4   13 P3.6/INT2/RxD_2
 CMP-/MCLKO/RST/P5.4 5   12 P3.3/INT1
		 Vcc 6   11 P3.2/INT0
	   CMP+/P5.5 7   10 P3.1/TxD/T2
		 Gnd 8    9 P3.0/RxD/INT4/T2CLKO
--------------------------------------------------------



~~~






install tool from linux
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

 USB-TxD---5(RxD)MK для stc15w204s DIP8

 USB-RxD---6(TxD)MK ...

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

описание STC15-English.pdf, там надо искать STC15W201S series MCU

про компилятор sdcc на сайте http://sdcc.sourceforge.net
http://sdcc.sourceforge.net/doc/sdccman.pdf
~~~





