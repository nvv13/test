источник
https://wiki.luatos.com/


project 

LuatOS + air105 + camera GC032A + TFT IPS 160x80 st7735s OR TFT IPS 240x240 st7789
                                                                                  
камера шла в комплекте с отладочной платой


https://github.com/nvv13/test/blob/main/test-mk/Air105/jpg/air105-back.jpg

отладочная плата
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/Air105/jpg/air105-back.jpg" alt="back side" title="back side" /></p>
фото 2
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/Air105/jpg/air105-top.jpg" alt="top side" title="top side" /></p>


------------------------------------------------


board air105 + камера GC032A + 0.96 TFT 80x160 дисплей st7735s SPI


Видео
https://youtu.be/cW-6fkVbjqQ


Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/0.96tft-back.jpg" alt="back side" title="back side" /></p>
фото 2
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

раскомментировать в скрипте 


spi_lcd = spi.deviceSetup(5,pin.PC14,0,0,8,48*1000*1000,spi.MSB,1,1) -- st7735s

log.info("lcd.init",
lcd.init("st7735s",{port = "device",pin_dc = pin.PE08 ,pin_rst = pin.PC12,pin_pwr = pin.PE09,direction = 2,w = 160,h = 80,xoffset = 1,yoffset = 26},spi_lcd))



------------------------------------------------


board HLK-W801-KIT-V1.1 + 1.3 TFT 240x240 дисплей st7789 SPI

Видео
https://www.youtube.com/watch?v=cW-6fkVbjqQ&t=96s

Дисплей
фото 1 
<p><img src="https://github.com/nvv13/test/blob/main/test-mk/w801/sdk-prj/01_UTFU/jpg/1.3tft-back.jpg" alt="back side" title="back side" /></p>
фото 2
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


раскомментировать в скрипте 

spi_lcd = spi.deviceSetup(5,pin.PC14,1,1,8,48*1000*1000,spi.MSB,1,1) -- st7789 CPHA=1 CPOL=1 current controller

log.info("lcd.init",
lcd.init("st7789",{port = "device",pin_dc = pin.PE08 ,pin_rst = pin.PC12,pin_pwr = pin.PE09,direction = 0,w = 240,h = 240,xoffset = 0,yoffset = 0},spi_lcd))


------------------------------------------------------


