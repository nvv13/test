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

// объявляем объект myGLCD класса библиотеки UTFT указывая тип дисплея
UTFT myGLCD(TFT01_24SP, dispMISO,
             dispSCK, dispCS,
             dispRST, dispDC);

void setup(){
    // инициируем дисплей
    myGLCD.InitLCD();
    myGLCD.clrScr();
}

void loop(){

    // заливаем дисплей красным, ждём 1 секунду
    myGLCD.fillScr(VGA_RED   );
    delay(1000);

    // заливаем дисплей зелёным, ждём 1 секунду
    myGLCD.fillScr(VGA_GREEN );
    delay(1000);

    // заливаем дисплей синим, ждём 1 секунду
    myGLCD.fillScr(VGA_BLUE  );
    delay(1000);

    // заливаем дисплей серебристым, ждём 1 секунду
    myGLCD.fillScr(VGA_SILVER);
    delay(1000);

    // заливаем дисплей бордовым, ждём 1 секунду
    myGLCD.fillScr(VGA_MAROON);
    delay(1000);

    // заливаем дисплей тём. синим, ждём 1 секунду
    myGLCD.fillScr(VGA_NAVY  );
    delay(1000);
}
