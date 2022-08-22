project

board HLK-W801-KIT-V1.1 + st7920



соединения

дисплей

LCD display st7920

надо соеденить по схеме:
~~~
connect to
st7920   W801  descreption
------   ----  ------------------------------------------------------
CS(17)   PB21  (Serial mode: Chip select.
				1: chip enabled;
				0: chip disabled.
				When chip is disabled, SID and SCLK
				should be set as “H” or “L”. Transcient
				of SID and SCLK is not allowed.)
SCLK(19) PB22  (Serial Mode: Serial clock.)
SID(18)  PB23  (Serial Mode: Sserial data input.)

PSB(23)  GND.  (0: serial mode st7920)

VCC      VCC
GND      GND


~~~







сборка (кратко):

1) качаем

   https://github.com/droppingy/W801_SDK-linux.git

   там же есть инструкции как его собирать, или читаем https://github.com/nvv13/test/blob/main/test-mk/w801/README.md


2) заменяем папку app на нашу app из проекта


3) далее сборка make, и прошивка make flash



  далее подцепляемся к платке, если интересует вывод на консоль

$ picocom --echo -b 115200 /dev/ttyUSB0

  после подсоединения платка "висит", это из за сигнала RTS (про это есть обьяснение в статье https://www.cnblogs.com/milton/p/15621540.html)
  поэтому делаем CTRL + A + G   - это комманда picocom - RTS down, (нажимать надо достаточно быстро, (одновременно, но сначала CTRL))
  и пользуемся ...
          выход из picocom - CTRL + A + Q  (это выход без сброса порта (Skipping tty reset...))





PS:
исходники драйвера ST7920 - https://github.com/hepingood/st7920






