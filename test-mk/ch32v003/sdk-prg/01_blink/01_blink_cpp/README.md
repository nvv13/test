
Платка CH32V003 или её еще просто обзывают TWen32F003 на МК CH32V003F4P6

<p><img src="https://github.com/nvv13/test/blob/main/test-mk/ch32v003/doc/TWen32F003.jpg" alt="TWen32F003" title="CH32V003" /></p>



соединения

надо соединить по схеме:
~~~


connect to
v003   WCH-LinkE
-----  ------ 
PD5    RX  
SWD    SWDIO
V      3V3
G      GND    
-----  ------ 


connect to
v003   LED
-----  ------ 
PA1    LED + резистор 300 Ом - 1 Ком на землю
GND    GND    
-----  ------ 



