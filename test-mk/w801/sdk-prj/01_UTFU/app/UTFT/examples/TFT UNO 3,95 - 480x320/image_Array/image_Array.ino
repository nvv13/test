//Цветной графический дисплей 3.95 TFT 480x320 UNO http://iarduino.ru/shop/Displei/cvetnoy-graficheskiy-displey-3-95-tft-480x320-uno-sensornyy.html

#include <UTFT.h>                                   // подключаем библиотеку UTFT для работы с дисплеем
UTFT myGLCD(TFT395UNO,A2,A1,A3,A4,A0);              // объявляем объект myGLCD класса библиотеки UTFT указывая тип дисплея TFT395UNO и номера выводов Arduino к которым подключён дисплей: RS, WR, CS, RST, SER. Выводы параллельной шины данных не указываются
extern unsigned short image_128x64[0x2000];         // объявляем массив image_128x64 который был определён во внешнем файле
                                                    //
void setup(){                                       //
   myGLCD.InitLCD();                                // инициируем дисплей
   myGLCD.clrScr();                                 // стираем всю информацию с дисплея
   myGLCD.drawBitmap(176,128,128,64,image_128x64);  // выводим на дисплей картинку начиная с координаты 176,128 размером 128,64 из массива image_128x64
}                                                   //
                                                    //
void loop(){}                                       //
