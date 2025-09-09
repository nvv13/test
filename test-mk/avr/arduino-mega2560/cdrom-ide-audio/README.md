в процессе


взято из [ArduinoCdPlayer](https://github.com/daniel1111/ArduinoCdPlayer.git)


[AtaInternal_pinout](https://pinoutguide.com/HD/AtaInternal_pinout.shtml)

![AtaInternal](AtaInternal.jpg)

connect to IDE
~~~

IDE	           	      IDE             IDE 
/reset   	   PL5 D44 -  1 2  - GND
Data7	 	   PA7 D29 -  3 4  - PC0 D37  Data8
    .	 	   PA6 D28 -  5 6  - PC1 D36  .
    .	 	   PA5 D27 -  7 8  - PC2 D35  .
    .	 	   PA4 D26 -  9 10 - PC3 D34  .
    .	 	   PA3 D25 - 11 12 - PC4 D33  .
    .	 	   PA2 D24 - 13 14 - PC5 D32  .
    .    	   PA1 D23 - 15 16 - PC6 D31  .
Data0	           PA0 D22 - 17 18 - PC7 D30  Data15
	               GND - 19 20 -x          KEY
DMARQ	                  x- 21 22 - GND
Write Strobe /DIOW PL6 D43 - 23 24 - GND
Read Strobe  /DIOR PL7 D42 - 25 26 - GND
I/O Ready                 x- 27 28 -x         SPSYNC:CSEL
/DMACK                    x- 29 30 - GND
INTRQ                     x- 31 32 -x 
Address 1  	   PL1 D48 - 33 34 -x 
Address 0          PL0 D49 - 35 36 - PL2 D47  Address 2
(1F0-1F7) /IDE_CS0 PL3 D46 - 37 38 - PL4 D45  /IDE_CS1 (3F6-3F7)
          /ACTIVE         x- 39 40 - GND
IDE	           	      IDE             IDE 




~~~


