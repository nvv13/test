
project

board HLK-W801-KIT-V1.1 + tm1637



соединения

дисплей

LCD display tm1637
     4 pin

надо соеденить по схеме:
~~~
connect to
W801  tm1637
GPIO  PIN
PB_21 CLK
PB_22 DIO
VCC   VCC
GND   GND


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
исходники драйвера tm1637 - https://github.com/avishorp/TM1637.



