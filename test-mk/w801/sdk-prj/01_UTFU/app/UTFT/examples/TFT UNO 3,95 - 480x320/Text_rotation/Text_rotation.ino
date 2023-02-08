//Цветной графический дисплей 3.95 TFT 480x320 UNO http://iarduino.ru/shop/Displei/cvetnoy-graficheskiy-displey-3-95-tft-480x320-uno-sensornyy.html

#include <UTFT.h>                              // подключаем библиотеку UTFT
UTFT myGLCD(TFT395UNO,A2,A1,A3,A4,A0);         // объявляем объект myGLCD класса библиотеки UTFT указывая тип дисплея TFT395UNO и номера выводов Arduino к которым подключён дисплей: RS, WR, CS, RST, SER. Выводы параллельной шины данных не указываются
extern uint8_t BigFont[];                      // подключаем большой шрифт
                                               //
void setup(){                                  //
   myGLCD.InitLCD();                           // инициируем дисплей
}                                              //
                                               //
void loop(){                                   //
   myGLCD.setFont(BigFont);                    // устанавливаем большой шрифт
   myGLCD.clrScr();                            // стираем всю информацию с дисплея
   myGLCD.setColor(0, 0, 255);                 // устанавливаем синий цвет текста
   myGLCD.print("0 degrees", 0, 0, 0);         // выводим текст "0 degrees" на дисплей начиная с точки 0x0 с поворотом на 0 градусов
   myGLCD.setColor(0, 255, 0);                 // устанавливаем зелёный цвет текста
   myGLCD.print("90 degrees",460,0,90);        // выводим текст "90 degrees" на дисплей начиная с точки 460x0 с поворотом на 90 градусов
   myGLCD.setColor(255, 0, 0);                 // устанавливаем красный цвет текста
   myGLCD.print("180 degrees",460,239,180);    // выводим текст "180 degrees" на дисплей начиная с точки 460x239 с поворотом на 180 градусов
   myGLCD.setColor(0, 255, 255);               // устанавливаем голубой цвет текста
   myGLCD.print("270 degrees",0,239,270);      // выводим текст "270 degrees" на дисплей начиная с точки 0x239 с поворотом на 270 градусов
   delay(20000);                               //
}                                              //
