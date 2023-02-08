//Цветной графический дисплей 1,8 TFT 128x160     http://iarduino.ru/shop/Displei/cvetnoy-graficheskiy-tft-ekran-128-10---1-8-rdquo.html

#include <UTFT.h>                              // подключаем библиотеку UTFT
UTFT myGLCD(TFT18SHLD,5,4,8,7,6);              // объявляем объект myGLCD класса библиотеки UTFT указывая тип дисплея TFT18SHLD и номера выводов Arduino к которым подключён дисплей: SDA, SCK, CS, RESET, A0. Можно использовать любые выводы Arduino.
extern uint8_t SmallFont[];                    // подключаем маленький шрифт
                                               //
void setup(){                                  //
   myGLCD.InitLCD();                           // инициируем дисплей
}                                              //
                                               //
void loop(){                                   //
   myGLCD.setFont(SmallFont);                  // устанавливаем маленький шрифт
   myGLCD.clrScr();                            // стираем всю информацию с дисплея
   myGLCD.setColor(0, 0, 255);                 // устанавливаем синий цвет текста
   myGLCD.print("0 degrees", 0, 0, 0);         // выводим текст "0 degrees"   на дисплей начиная с точки   0x0   и поворотом на 0°
   myGLCD.setColor(0, 255, 0);                 // устанавливаем зелёный цвет текста
   myGLCD.print("90 degrees",160,0,90);        // выводим текст "90 degrees"  на дисплей начиная с точки 160x0   и поворотом на 90°
   myGLCD.setColor(255, 0, 0);                 // устанавливаем красный цвет текста
   myGLCD.print("180 degrees",160,128,180);    // выводим текст "180 degrees" на дисплей начиная с точки 160x128 и поворотом на 180°
   myGLCD.setColor(0, 255, 255);               // устанавливаем голубой цвет текста
   myGLCD.print("270 degrees",0,128,270);      // выводим текст "270 degrees" на дисплей начиная с точки   0x128 и поворотом на 270°
   delay(20000);                               //
}                                              //
