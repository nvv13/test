//Цветной графический дисплей 1,8 TFT 128x160     http://iarduino.ru/shop/Displei/cvetnoy-graficheskiy-tft-ekran-128-10---1-8-rdquo.html

#include <UTFT.h>                              // подключаем библиотеку UTFT
UTFT myGLCD(TFT18SHLD,5,4,8,7,6);              // объявляем объект myGLCD класса библиотеки UTFT указывая тип дисплея TFT18SHLD и номера выводов Arduino к которым подключён дисплей: SDA, SCK, CS, RESET, A0. Можно использовать любые выводы Arduino.
extern uint8_t SmallFont[];                    // подключаем маленький шрифт
extern uint8_t BigFont[];                      // подключаем большой шрифт
extern uint8_t SevenSegNumFont[];              // подключаем шрифт имитирующий семисегментный индикатор
                                               //
void setup(){                                  //
   myGLCD.InitLCD();                           // инициируем дисплей
}                                              //
                                               //
void loop(){                                   //
   myGLCD.clrScr();                            // стираем всю информацию с дисплея
                                               //
   myGLCD.setFont(SmallFont);                  // устанавливаем маленький шрифт
   myGLCD.setColor(VGA_WHITE);                 // устанавливаем белый цвет текста
   myGLCD.print("iarduino.ru", CENTER, 10);    // выводим текст на дисплей (выравнивание по ширине - центр дисплея, координата по высоте 10 точек)
                                               //
   myGLCD.setFont(BigFont);                    // устанавливаем большой шрифт
   myGLCD.setColor(VGA_BLUE);                  // устанавливаем синий цвет текста
   myGLCD.print("BigFont", CENTER, 40);        // выводим текст на дисплей (выравнивание по ширине - центр дисплея, координата по высоте 40 точек)
                                               //
   myGLCD.setFont(SevenSegNumFont);            // устанавливаем шрифт имитирующий семисегментный индикатор
   myGLCD.setColor(VGA_FUCHSIA);               // устанавливаем пурпурный цвет текста
   myGLCD.print("98765", CENTER, 70);          // выводим текст на дисплей (выравнивание по ширине - центр дисплея, координата по высоте 70 точек)
                                               //
   delay(20000);                               // ждём 20 секунд
}                                              //
