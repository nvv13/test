!!!В РАБОТЕ!!!
!!!underway!!!


project

board HLK-W801-KIT-V1.1 + ws2812b RGB address LED + wifi + OTA upgrade 

~~~

Удаленное управление: можно подцепиться к платке по сети, на порт 5555,
 пример программки для соединения chat_client.py
 команды
          help - данная справка (flash ver- )
          time - время и прочие состояния
          drX - режим эф.ленты, где X, только 1-, 2-, 3-выкл.
          spd=A - где A - скорость
          mode=Y - режим драйвера ленты, где Y, 0-PIN_MODE, SPI_MODE: 
                                          1-3bit, 2-4bit, 3-5bit, 4-6bit, 5-7bit, 6-8bit
          on=Z, где Z - бит для 1 в SPI_MODE, можно менять для эксперементов 
          от 0 до 8
          off=V,где V - бит для 0
          clk=K,где K - частота в Герцах для SPI_MODE
          upgrade - обновить прошивку по OTA, лезет на "http://192.168.1.69/ota/ws2812b/w800_ota.img"


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









