project 

LuatOS + air105 + camera GC032A + TFT IPS 160x80 st7735s OR TFT IPS 240x240 st7789
                                                                                  
камера шла в комплекте с отладочной платой

------------------------------------------------


board air105 + камера GC032A + 0.96 TFT 80x160 дисплей st7735s SPI


Видео
https://youtu.be/cW-6fkVbjqQ


Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/0.96tft-back.jpg" alt="back side" title="back side" /></p>
фото2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/0.96tft.jpg" alt="top side" title="top side" /></p>


соединения

надо соеденить по схеме:
~~~
LCD pins Air105 pins
GND      GND
VCC      3.3V
SCL      (PC15/HSPI_SCK)
SDA      (PC13/HSPI_MOSI)
RES      (PC12/HSPI_MISO)
DC       (PE08) --U3_RX on the development board
CS       (PC14/HSPI_CS)
BL       (PE09) --U3_TX on the development board
~~~



------------------------------------------------


board HLK-W801-KIT-V1.1 + 1.3 TFT 240x240 дисплей st7789 SPI

Видео
https://youtu.be/cW-6fkVbjqQ

Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/1.3tft-back.jpg" alt="back side" title="back side" /></p>
фото2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/1.3tft.jpg" alt="top side" title="top side" /></p>


соединения

надо соеденить по схеме:
~~~
LCD pins Air105 pins
GND      GND
VCC      3.3V
SCL      (PC15/HSPI_SCK)
SDA      (PC13/HSPI_MOSI)
RES      (PC12/HSPI_MISO)
DC       (PE08) --U3_RX on the development board
BL       (PE09) --U3_TX on the development board
~~~





