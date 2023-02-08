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

// объявляем массив image_128x64 который был определён во внешнем файле
extern unsigned short image_128x64[0x2000];
// объявляем объект myGLCD класса библиотеки UTFT указывая тип дисплея
UTFT myGLCD(TFT01_24SP, dispMISO,
             dispSCK, dispCS,
             dispRST, dispDC);
void setup(){
    // инициируем дисплей
    myGLCD.InitLCD();
    // стираем всю информацию с дисплея
    myGLCD.clrScr();
    // выводим на дисплей картинку начиная
    // с координаты 176,128 размером 128,64
    // из массива image_128x64
    myGLCD.drawBitmap(176,128,128,64,image_128x64);
}

void loop(){}
