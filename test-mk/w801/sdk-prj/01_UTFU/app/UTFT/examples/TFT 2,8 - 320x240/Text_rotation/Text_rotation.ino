// Цветной графический дисплей 2.8 TFT 320x240 https://iarduino.ru/shop/Displei/cvetnoy-graficheskiy-displey-2-8-tft-320x240.html
// подключаем библиотеку UTFT
#include <UTFT.h>

// Определяем выводы используемые для
// управления дисплеем 2.8" TFT 320x240 UNO:
#define dispMISO    8
#define dispSCK     7
#define dispCS      6
#define dispRST     5
#define dispDC      4

// подключаем шрифт
extern uint8_t BigFont[];

// объявляем объект myGLCD класса библиотеки UTFT указывая тип дисплея
UTFT myGLCD(TFT01_24SP, dispMISO,
             dispSCK, dispCS,
             dispRST, dispDC);

void setup(){
    // инициируем дисплей
    myGLCD.InitLCD();
}

void loop(){

    // устанавливаем большой шрифт
    myGLCD.setFont(BigFont);
    // стираем всю информацию с дисплея
    myGLCD.clrScr();
    // устанавливаем синий цвет текста
    myGLCD.setColor(0, 0, 255);

    // выводим текст "0 degrees" на дисплей начиная
    // с точки 0x0 с поворотом на 0 градусов
    myGLCD.print((char*)"0 degrees", 0, 0, 0);

    // устанавливаем зелёный цвет текста
    myGLCD.setColor(0, 255, 0);

    // выводим текст "90 degrees" на дисплей начиная
    // с точки 319x0 с поворотом на 90 градусов
    myGLCD.print((char*)"90 degrees",319,0,90);

    // устанавливаем красный цвет текста
    myGLCD.setColor(255, 0, 0);

    // выводим текст "180 degrees" на дисплей начиная
    // с точки 319x239 с поворотом на 180 градусов
    myGLCD.print((char*)"180 degrees",319,239,180);

    // устанавливаем голубой цвет текста
    myGLCD.setColor(0, 255, 255);

    // выводим текст "270 degrees" на дисплей начиная
    // с точки 0x239 с поворотом на 270 градусов
    myGLCD.print((char*)"270 degrees",0,239,270);

    delay(20000);
}
