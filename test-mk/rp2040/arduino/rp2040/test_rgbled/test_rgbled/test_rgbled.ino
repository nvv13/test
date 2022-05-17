#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
// use on-board neopixel PIN_NEOPIXEL if existed
#ifndef PIN_NEOPIXEL
  #define PIN_NEOPIXEL 8
#endif

#define NUM_LEDS 394 //60*5

// How many NeoPixels are attached to the Arduino?
// use on-board defined NEOPIXEL_NUM if existed
#ifndef NEOPIXEL_NUM
  #define NEOPIXEL_NUM  NUM_LEDS
#endif

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel leds = Adafruit_NeoPixel(NEOPIXEL_NUM, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);


#define SERIAL1_PIN_RX D1
#define SERIAL1_PIN_TX D0

/* Define digital pins useto read the encoder */
#define DT  D19 //2
#define CLK D20 //3
#define SW  D21 //4
  int counter;
  byte DialPos;
  byte Last_DialPos;
 int iReset;

//#define xPin  A1
//#define yPin  A0
#define buttonPin SW //2
int xPosition = 0;
int yPosition = 0;
int buttonState = 0;

#define MODE_COLOR 0
#define MODE_OFF   1
#define MODE_WHILE 2
#define MODE_MAX 2
#define MODE_SERIAL 3
#define MODE_CYCLE 4
#define MODE_VAVE 5
#define MODE_MAX_ALL 5

//#define CRGB uint8_t

uint32_t CRGB(uint8_t red,uint8_t green,uint8_t blue)
{
uint32_t color = (red << 16) | (green << 8) | blue;  
return color;  
}

uint32_t CHSV(uint8_t ih, uint8_t is, uint8_t iv)
{
uint32_t color = (ih << 16) | (is << 8) | iv;  
return color;  
}

       

static uint8_t i_hue, i_bright;
int i_Mode = MODE_WHILE;
int i_trimm_delay = 0;
int i_hue_delay = 0;
uint32_t i_set = CRGB( 0, 0, 0);
uint32_t i_set_last;
int  i_Mode_last = 0;
int  i_zone_last = 0;

#define LEN_BUF1 64 //не меньше!
char buf1[LEN_BUF1];
byte indexBuf1 = 0;

#define INTERVAL_S                100   // ms 
unsigned long secTime = millis();

int i_zone = 0;
//const PROGMEM - not working!
#define MAX_ZONE 11
const int zone [MAX_ZONE][84]   =
{
  /* zone[x] где x - № зоны   */
  /* в 0 позиции каждой зоны (zone[x][0]) - количество светодиодов */
  /* зоны 1-9, описание всей rgb Ленты на кухне */


  /* z1 -> верх к окну, 30 на метр */
  {37,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36},
  /* z2 <- низ от окна, 60 на метр */
  {82,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118},
  /* z3 | низ на меня, 60 на метр */
  {17, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135},
  /* z4 -> низ к окну, 60 на метр */
  {83, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218},

  /* z5 <- верх от окна, 30 на метр */
  {48, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266},
  /* z6 -> низ к окну, 60 на метр */
  {45, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311},
  /* z7 | низ на меня, 60 на метр */
  {14, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325},
  /* z8 <- низ от окна, 60 на метр */
  {48, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370},
  /* z9 | низ от меня, 60 на метр */
  {14, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383, 384},

  /* z10  через 10 */
  {39,   1,  11,  21,  31,  41,  51,  61,  71,  81,  91, 101, 111, 121, 131, 141, 151, 161, 171, 181, 191, 201, 211, 221, 231, 241, 251, 261, 271, 281, 291, 301, 311, 321, 331, 341, 351, 361, 371, 381},
  /* z11  через 20 */
  {20,   1,   5,  25,  45,  65,  85, 105, 125, 145, 165, 185, 205, 225, 245, 265, 285, 305, 325, 345, 365}


};

int i_shift = 0;
int i_Delay = 500;
int i_debag = 0;

int i_l_Green = 0, i_l_Red = 0, i_l_Blue = 0;



// The normal, core0 setup
void setup() {
  Serial.begin(115200);

/*
``Serial`` is the ACM-CDC serial port, and while ``Serial.begin()`` 
The RP2040 provides two hardware-based UARTS with configurable pin selection.
``Serial1`` is ``UART0``, and ``Serial2`` is ``UART1``.
Configure their pins using the ``setXXX`` calls prior to calling ``begin()``
.. code:: cpp
        Serial1.setRX(pin);  // aviable pins, to see rp2040_board_pinout.jpeg !
        Serial1.setTX(pin);
        //Serial1.setRTS(pin); // if need
        //Serial1.setCTS(pin);
        Serial1.begin(baud);
*/

  Serial1.setRX(SERIAL1_PIN_RX);
  Serial1.setTX(SERIAL1_PIN_TX);
  Serial1.begin(9600);
  Serial1.println(F("LED start"));
  
  // This initializes the NeoPixel with RED
#ifdef NEOPIXEL_POWER
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, NEOPIXEL_POWER_ON);
#endif

  leds.begin();
  leds.setBrightness(50);
  leds.fill(0xff0000);
  leds.show();

    
  pinMode(DT, INPUT);   
  pinMode(CLK, INPUT);  
  //pinMode(SW, INPUT);  
  //digitalWrite(SW, HIGH);
  pinMode(buttonPin, INPUT_PULLUP);
  iReset=1;
  
  Reset01();
}


void Reset01(void) // ON initial
{
  i_Mode = MODE_WHILE;
  i_Mode_last = i_Mode;
  i_zone=0;  
  i_bright = 075;
  i_set_last = i_set;
  i_set = CRGB( i_bright, i_bright, i_bright);
  SetLED();
        //  green, red, blue
  i_set = CRGB( 185, 185, 185);
  i_zone=1;
  SetLED();
  i_zone=5;
  SetLED();
  
  i_bright = 180;
  i_set = CRGB( i_bright+10, i_bright+10, i_bright);
  i_zone=8;
  SetLED();
  i_zone=6;
  SetLED();

  i_zone=0;
  i_l_Green = i_bright, i_l_Red = i_bright, i_l_Blue = i_bright;
  i_Mode_last = i_Mode;
  i_zone_last = i_zone;
  i_set_last = i_set;
}

void Reset02(void) // OFF button LED
{
  i_Mode = MODE_WHILE;
  i_Mode_last = i_Mode;
  i_zone=0;
  i_bright = 000;
  i_set_last = i_set;
  i_set = CRGB( i_bright, i_bright, i_bright);
  SetLED();
        //  green, red, blue
  i_set = CRGB( 185, 185, 185);
  i_zone=1;
  SetLED();
  i_zone=5;
  SetLED();
  
  i_zone=0;
  i_l_Green = i_bright, i_l_Red = i_bright, i_l_Blue = i_bright;
  i_set =  CRGB(i_l_Green, i_l_Red, i_l_Blue);
  i_Mode_last = i_Mode;
  i_zone_last = i_zone;
  i_set_last = i_set;
}

void Reset02_Z02(void) // OFF button LED  ON Z2 Z3 
{
  i_Mode = MODE_WHILE;
  i_Mode_last = i_Mode;
  i_zone=0;
  i_bright = 000; //off
  i_set_last = i_set;
  i_set = CRGB( i_bright, i_bright, i_bright);
  SetLED();
  
        //  green, red, blue ON TOP
  i_set = CRGB( 185, 185, 185);
  i_zone=1;
  SetLED();
  i_zone=5;
  SetLED();
  
  // ON
  i_zone=2;
  SetLED();

  i_zone=0;
  i_l_Green = i_bright, i_l_Red = i_bright, i_l_Blue = i_bright;
  i_set =  CRGB(i_l_Green, i_l_Red, i_l_Blue);
  i_Mode_last = i_Mode;
  i_zone_last = i_zone;
  i_set_last = i_set;
}

void Reset02_Z03_Z04(void) // OFF button LED  ON Z3 Z4 
{
  i_Mode = MODE_WHILE;
  i_Mode_last = i_Mode;
  i_zone=0;
  i_bright = 000; //off
  i_set_last = i_set;
  i_set = CRGB( i_bright, i_bright, i_bright);
  SetLED();
  
        //  green, red, blue ON TOP
  i_set = CRGB( 185, 185, 185);
  i_zone=1;
  SetLED();
  i_zone=5;
  SetLED();
  
  // ON
  i_zone=3;
  SetLED();
  i_zone=4;
  SetLED();

  i_zone=0;
  i_l_Green = i_bright, i_l_Red = i_bright, i_l_Blue = i_bright;
  i_set =  CRGB(i_l_Green, i_l_Red, i_l_Blue);
  i_Mode_last = i_Mode;
  i_zone_last = i_zone;
  i_set_last = i_set;
}

void Reset02_Z03_Z09(void) // OFF button LED  ON Z3 Z4 
{
  i_Mode = MODE_WHILE;
  i_Mode_last = i_Mode;
  i_zone=0;
  i_bright = 000; //off
  i_set_last = i_set;
  i_set = CRGB( i_bright, i_bright, i_bright);
  SetLED();
  
        //  green, red, blue ON TOP
  i_set = CRGB( 185, 185, 185);
  i_zone=1;
  SetLED();
  i_zone=5;
  SetLED();
  
  // ON
  i_zone=3;
  SetLED();
  i_zone=9;
  SetLED();

  i_zone=0;
  i_l_Green = i_bright, i_l_Red = i_bright, i_l_Blue = i_bright;
  i_set =  CRGB(i_l_Green, i_l_Red, i_l_Blue);
  i_Mode_last = i_Mode;
  i_zone_last = i_zone;
  i_set_last = i_set;
}


void Reset02_Z02_Z03_Z04(void) // OFF button LED  ON Z02 Z3 Z4 
{
  i_Mode = MODE_WHILE;
  i_Mode_last = i_Mode;
  i_zone=0;
  i_bright = 000; //off
  i_set_last = i_set;
  i_set = CRGB( i_bright, i_bright, i_bright);
  SetLED();
  
        //  green, red, blue ON TOP
  i_set = CRGB( 185, 185, 185);
  i_zone=1;
  SetLED();
  i_zone=5;
  SetLED();
  
  // ON
  i_zone=2;
  SetLED();
  i_zone=3;
  SetLED();
  i_zone=4;
  SetLED();

  i_zone=0;
  i_l_Green = i_bright, i_l_Red = i_bright, i_l_Blue = i_bright;
  i_set =  CRGB(i_l_Green, i_l_Red, i_l_Blue);
  i_Mode_last = i_Mode;
  i_zone_last = i_zone;
  i_set_last = i_set;
}

void Reset02_Z06_Z07_Z08_Z09(void) // OFF button LED  ON Z06 Z7 Z8 
{
  i_Mode = MODE_WHILE;
  i_Mode_last = i_Mode;
  i_zone=0;
  i_bright = 000; //off
  i_set_last = i_set;
  i_set = CRGB( i_bright, i_bright, i_bright);
  SetLED();
  
        //  green, red, blue ON TOP
  i_set = CRGB( 185, 185, 185);
  i_zone=1;
  SetLED();
  i_zone=5;
  SetLED();
  
  // ON
  i_zone=6;
  SetLED();
  i_zone=7;
  SetLED();
  i_zone=8;
  SetLED();
  i_zone=9;
  SetLED();

  i_zone=0;
  i_l_Green = i_bright, i_l_Red = i_bright, i_l_Blue = i_bright;
  i_set =  CRGB(i_l_Green, i_l_Red, i_l_Blue);
  i_Mode_last = i_Mode;
  i_zone_last = i_zone;
  i_set_last = i_set;
}

void Reset03(void)
{
  i_Mode = MODE_SERIAL;
  i_Mode_last = i_Mode;
  i_zone=0;
  i_bright = 185;
  i_l_Green = 235, i_l_Red = 0, i_l_Blue = 0;
  i_set =  CRGB(i_l_Green, i_l_Red, i_l_Blue);
  i_set_last = i_set;
  SetLED();
        //  green, red, blue
  i_set = CRGB( 185, 185, 185);
  i_zone=1;
  SetLED();
  i_zone=5;
  SetLED();
  
  i_zone=0;
  i_l_Green = 235, i_l_Red = 0, i_l_Blue = 0;
  i_set =  CRGB(i_l_Green, i_l_Red, i_l_Blue);
  i_Mode_last = i_Mode;
  i_zone_last = i_zone;
  i_set_last = i_set;
}

void Reset04(void)
{
  i_Mode = MODE_SERIAL;
  i_Mode_last = i_Mode;
  i_zone=0;
  i_bright = 185;
  i_l_Green = 0, i_l_Red = 235, i_l_Blue = 0;
  i_set =  CRGB(i_l_Green, i_l_Red, i_l_Blue);
  i_set_last = i_set;
  SetLED();
        //  green, red, blue
  i_set = CRGB( 185, 185, 185);
  i_zone=1;
  SetLED();
  i_zone=5;
  SetLED();
  
  i_zone=0;
  i_l_Green = 0, i_l_Red = 235, i_l_Blue = 0;
  i_set =  CRGB(i_l_Green, i_l_Red, i_l_Blue);
  i_Mode_last = i_Mode;
  i_zone_last = i_zone;
  i_set_last = i_set;
}

void Reset05(void)
{
  i_Mode = MODE_SERIAL;
  i_Mode_last = i_Mode;
  i_zone=0;
  i_bright = 185;
  i_l_Green = 0, i_l_Red = 0, i_l_Blue = 235;
  i_set =  CRGB(i_l_Green, i_l_Red, i_l_Blue);
  i_set_last = i_set;
  SetLED();
        //  green, red, blue
  i_set = CRGB( 185, 185, 185);
  i_zone=1;
  SetLED();
  i_zone=5;
  SetLED();
  
  i_zone=0;
  i_l_Green = 0, i_l_Red = 0, i_l_Blue = 235;
  i_set =  CRGB(i_l_Green, i_l_Red, i_l_Blue);
  i_Mode_last = i_Mode;
  i_zone_last = i_zone;
  i_set_last = i_set;
}

// 0.8 amper 60 leds WHILE
// 0.05 amper OFF

void CheckJoystik(void)
{
  //xPosition = analogRead(xPin);
  //yPosition = analogRead(yPin);
  buttonState = digitalRead(buttonPin);

    /* Read the status of the dial */
    DialPos = (digitalRead(CLK) << 1) | digitalRead(DT);

  
    /* Is the dial being turned anti-clockwise? */
    if (DialPos == 3 && Last_DialPos == 1)
    {
      counter--;
    }
  
    /* Is the dial being turned clockwise? */
    if (DialPos == 3 && Last_DialPos == 2)
    {
      counter++;
    }

    Last_DialPos = DialPos;
    
  /*
      Serial1.print("X: ");
      Serial1.print(xPosition);
      Serial1.print(" | Y: ");
      Serial1.print(yPosition);
      Serial1.print(" | Button: ");
      Serial1.println(buttonState);
    */
    //delay(100); // добавляем задержку между считыванием данных


    /*
      Red (0..) "HUE_RED"
      Orange (32..) "HUE_ORANGE"
      Yellow (64..) "HUE_YELLOW"
      Green (96..) "HUE_GREEN"
      Aqua (128..) "HUE_AQUA"
      Blue (160..) "HUE_BLUE"
      Purple (192..) "HUE_PURPLE"
      Pink(224..) "HUE_PINK"
      Often in other HSV color spaces, hue is represented as an angle from 0-360 degrees. But for compactness, efficiency, and speed, this library represents hue as a single-byte number from 0-255. There's a full wiki page how FastLED deals with HSV colors here.

      "saturation" is a one-byte value ranging from 0-255, where 255 means "completely saturated, pure color", 128 means "half-saturated, a light, pale color", and 0 means "completely de-saturated: plain white".

      "value" is a one-byte value ranging from 0-255 representing brightness, where 255 means "completely bright, fully lit", 128 means "somewhat dimmed, only half-lit", and zero means "completely dark: black."
  */

/*
  if (counter > 0
      //&& i_hue < 255
      && i_hue_delay == 0)
  {
    i_hue+=counter;
    i_hue_delay = 1                                                                                                                     ;
  }
  if (counter<0 
      //&& i_hue > 0
      && i_hue_delay == 0)
  {
    i_hue+=counter;
    i_hue_delay = 1;
  }
*/

  if (counter > 1 && i_bright < 200)
  {
    i_bright+=3;
  }
  if (counter<-1  && i_bright > 10)
  {
    i_bright-=3;
  }

  if (buttonState == 0 && i_trimm_delay == 0)
  {
    switch(iReset)
    {
     case 1: Reset01();break;//on
     case 2: Reset02();break;//of
     case 3: Reset02_Z02();break;//Reset03();break;
     case 4: Reset02_Z02_Z03_Z04();break;//Reset04();break;
     case 5: Reset02_Z06_Z07_Z08_Z09();break;//Reset05();break;
    }
    if(iReset++>5)iReset=1;
    i_trimm_delay = 10000;
  }

  if (i_trimm_delay > 0)i_trimm_delay--;
  if (i_hue_delay > 0)i_hue_delay--;

    /* Reset the counter */
   if(counter>1 || counter<-1) counter = 0;

}

void loop() {

  //Набросок для приема комманд по serial порту
  while (Serial1.available() > 0)
  {
    char cRead = Serial1.read();
    if (cRead == '\n' || cRead == '\r') {
      buf1[indexBuf1++] = 0; //комманда в буффере!
      //Serial1.println(buf1);
      if (strcmp(buf1, "reset") == 0)
      {
        Serial1.println(F("cmd reset"));
        delay(500);
        //resetFunc();
      }

      char *interM = strstr(buf1, "m");
      if (interM != NULL)
      {
        Serial1.print(F("cmd "));
        Serial1.println(interM);
        i_Mode = atoi(interM + 1) ;
        if (i_Mode > MODE_MAX_ALL)i_Mode = i_Mode_last;
      }

      char *interP = strstr(buf1, "!");
      if (interP != NULL)
      {
        Serial1.print(F("cmd "));
        Serial1.println(interP);
        i_bright = atoi(interP + 1);
        if (i_Mode != MODE_WHILE || i_Mode != MODE_COLOR)
          i_Mode = MODE_WHILE ;
        i_l_Green = i_bright, i_l_Red = i_bright, i_l_Blue = i_bright;
      }
      char *interG = strstr(buf1, "*");
      if (interG != NULL)
      {
        Serial1.print(F("cmd "));
        Serial1.println(interG);
        i_hue = atoi(interG + 1);
        i_Mode = MODE_COLOR;
      }
      char *interH = strstr(buf1, ":");
      if (interH != NULL)
      {
        Serial1.print(F("cmd "));
        Serial1.println(interH);
        interH[4] = 0;
        interH[8] = 0;
        //  green, red, blue
        i_set = CRGB(atoi(interH + 5) , atoi(interH + 1), atoi(interH + 9));
        i_Mode = MODE_SERIAL;
      }
      char *interRed = strstr(buf1, "r");
      if (interRed != NULL)
      {
        Serial1.print(F("cmd "));
        Serial1.println(interRed);
        //  green, red, blue
        i_l_Red = atoi(interRed + 1);
        i_set =  CRGB(i_l_Green, i_l_Red, i_l_Blue);
        i_Mode = MODE_SERIAL;
      }
      char *interGreen = strstr(buf1, "g");
      if (interGreen != NULL)
      {
        Serial1.print(F("cmd "));
        Serial1.println(interGreen);
        //  green, red, blue
        i_l_Green = atoi(interGreen + 1);
        i_set =  CRGB(i_l_Green, i_l_Red, i_l_Blue);
        i_Mode = MODE_SERIAL;
      }
      char *interBlue = strstr(buf1, "b");
      if (interBlue != NULL)
      {
        Serial1.print(F("cmd "));
        Serial1.println(interBlue);
        //  green, red, blue
        i_l_Blue = atoi(interBlue + 1);
        i_set =  CRGB(i_l_Green, i_l_Red, i_l_Blue);
        i_Mode = MODE_SERIAL;
      }

      char *interZ = strstr(buf1, "z");
      if (interZ != NULL)
      {
        Serial1.print(F("cmd "));
        Serial1.println(interZ);
        i_zone = atoi(interZ + 1) ;
        if (i_zone > MAX_ZONE)i_zone = i_zone_last;
      }

      char *interDelay = strstr(buf1, "d");
      if (interDelay != NULL)
      {
        Serial1.print(F("cmd "));
        Serial1.println(interDelay);
        i_Delay = atoi(interDelay + 1) ;
      }

      char *interdebag = strstr(buf1, "l");
      if (interdebag != NULL)
      {
        Serial1.print(F("cmd "));
        Serial1.println(interdebag);
        i_debag = atoi(interdebag + 1) ;
      }


      indexBuf1 = 0;
    }
    else
    {
      if (indexBuf1 < LEN_BUF1)
        buf1[indexBuf1++] = cRead;
      else
        indexBuf1 = 0;
    }
  }


  CheckJoystik();
  switch (i_Mode)
  {
    case MODE_COLOR:
      {
        i_set = CHSV(i_hue, 255, i_bright);
      }; break;
    case MODE_OFF:
      {
        i_set = CRGB( 0, 0, 0);
      }; break;
    case MODE_WHILE:
      {
        i_set = CRGB( i_bright, i_bright, i_bright);
      }; break;
    case MODE_SERIAL:
      {
        //if(i_bright==0)
        // i_bright=128;
        // let's set an led value
        //i_set = CHSV(i_hue, 255, i_bright);
      }; break;
    case  MODE_CYCLE:
      {
        i_zone = 0;
        uint32_t i_0 = leds.getPixelColor(0);
        for (int i = 0; i < NUM_LEDS - 1; i++) {
          leds.setPixelColor(i,leds.getPixelColor(i + 1));

        }
        leds.setPixelColor(NUM_LEDS - 1,i_0);
        delay(i_Delay);
      }; break;
    case  MODE_VAVE:
      {
        i_zone = 0;

        move30(1);
        re_move60(2);
        move60(4);
        re_move30(5);
        move60(6);
        re_move60(8);

        if (++i_shift > 20)i_shift = 0;
        delay(i_Delay);
      }; break;
  }

  if (i_set_last != i_set || i_Mode_last != i_Mode || i_zone_last != i_zone || i_Mode == MODE_CYCLE || i_Mode == MODE_VAVE)
  {
    SetLED();
  }
}

void iZoneSetLED(int izone)
{
    for (int i = 0; i < zone[izone - 1][0]; i++) {
      int iInd = zone[izone - 1][i + 1];
      leds.setPixelColor(iInd,i_set);
    }
}


void SetLED(void)
{
  if (i_debag == 1)
  {
    Serial1.print(F("i_zone="));
    Serial1.print(i_zone, DEC);
    Serial1.print(F(",i_Mode="));
    Serial1.print(i_Mode, DEC);
    Serial1.print(F(",i_bright="));
    Serial1.print(i_bright, DEC);
    Serial1.print(F(",i_hue="));
    Serial1.print(i_hue, DEC);
//    Serial1.print(F(",i_set.red="));
//    Serial1.print(i_set.green, DEC);
    //Serial1.print(F(",i_set.green="));
//    Serial1.print(i_set.red, DEC);
//    Serial1.print(F(",i_set.blue="));
//    Serial1.print(i_set.blue, DEC);
    Serial1.print(F(",i_shift="));
    Serial1.print(i_shift, DEC);
    Serial1.print(F(",i_Delay="));
    Serial1.println(i_Delay, DEC);
  }
  if (i_zone <= 0)
  {
    if (!(i_Mode == MODE_CYCLE || i_Mode == MODE_VAVE))
    {
      //for (int i = 0; i < NUM_LEDS; i++) {
      //  leds[i] = i_set;
      //}
      iZoneSetLED(2);
      iZoneSetLED(3);
      iZoneSetLED(4);
      iZoneSetLED(6);
      iZoneSetLED(7);
      iZoneSetLED(8);
      iZoneSetLED(9);
      
    }
    if (i_debag == 1)log_serial();
  }
  else
  {
    //Serial.print(zone[i_zone - 1][0], DEC);
    iZoneSetLED(i_zone);
    if (i_debag == 1)log_serial();
  }

  //FastLED.delay(0);
  leds.show();
  i_set_last = i_set;
  i_Mode_last = i_Mode;
  i_zone_last = i_zone;
}

void move30(int i_z_set)
{
  for (int i = 0; i < zone[i_z_set - 1][0]; i++) {
    int iInd = zone[i_z_set - 1][i + 1];
    if ( (-10 + i_shift / 2  <= i && i < -5 + i_shift / 2 ) || (0 + i_shift / 2 <= i && i < 5 + i_shift / 2) || (10 + i_shift / 2 <= i && i < 15 + i_shift / 2) || (20 + i_shift / 2 <= i && i < 25 + i_shift / 2) || (30 + i_shift / 2 <= i && i < 35 + i_shift / 2))
      leds.setPixelColor(iInd,i_set);
    else
      leds.setPixelColor(iInd,CRGB( 0, 0, 0));
  }
}

void move60(int i_z_set)
{
  for (int i = 0; i < zone[i_z_set - 1][0]; i++) {
    int iInd = zone[i_z_set - 1][i + 1];
    if ( (-20 + i_shift <= i && i < -10 + i_shift) || (0 + i_shift <= i && i < 10 + i_shift) || (20 + i_shift <= i && i < 30 + i_shift) || (40 + i_shift <= i && i < 50 + i_shift) || (60 + i_shift <= i && i < 70 + i_shift) || (80 + i_shift <= i && i < 90 + i_shift))
      leds.setPixelColor(iInd,i_set);
    else
      leds.setPixelColor(iInd,CRGB( 0, 0, 0));
  }

}

void re_move30(int i_z_set)
{
  int i_shift_revers = 20 - i_shift;
  for (int i = 0; i < zone[i_z_set - 1][0]; i++) {
    int iInd = zone[i_z_set - 1][i + 1];
    if ( (-10 + i_shift_revers / 2  <= i && i < -5 + i_shift_revers / 2 ) ||  (0 + i_shift_revers / 2 <= i && i < 5 + i_shift_revers / 2) || (10 + i_shift_revers / 2 <= i && i < 15 + i_shift_revers / 2) || (20 + i_shift_revers / 2 <= i && i < 25 + i_shift_revers / 2) || (30 + i_shift_revers / 2 <= i && i < 35 + i_shift_revers / 2))
      leds.setPixelColor(iInd,i_set);
    else
      leds.setPixelColor(iInd,CRGB( 0, 0, 0));
  }
}

void re_move60(int i_z_set)
{
  int i_shift_revers = 20 - i_shift;
  for (int i = 0; i < zone[i_z_set - 1][0]; i++) {
    int iInd = zone[i_z_set - 1][i + 1];
    if ( (-20 + i_shift_revers <= i && i < -10 + i_shift_revers) ||  (0 + i_shift_revers <= i && i < 10 + i_shift_revers) || (20 + i_shift_revers <= i && i < 30 + i_shift_revers) || (40 + i_shift_revers <= i && i < 50 + i_shift_revers) || (60 + i_shift_revers <= i && i < 70 + i_shift_revers) || (80 + i_shift_revers <= i && i < 90 + i_shift_revers))
      leds.setPixelColor(iInd,i_set);
    else
      leds.setPixelColor(iInd,CRGB( 0, 0, 0));
  }

}

void log_serial(void)
{
  Serial.println("");
  Serial.print(F("i_zone="));
  Serial.print(i_zone, DEC);
  Serial.print(F(",i_Mode="));
  Serial.print(i_Mode, DEC);
  Serial.print(F(",i_bright="));
  Serial.print(i_bright, DEC);
  Serial.print(F(",i_hue="));
  Serial.print(i_hue, DEC);
  //Serial.print(F(",i_set.red="));
  //Serial.print(i_set.green, DEC);
  //Serial.print(F(",i_set.green="));
  //Serial.print(i_set.red, DEC);
  //Serial.print(F(",i_set.blue="));
  //Serial.println(i_set.blue, DEC);
  Serial.println("");
}



// Running on core1
void setup1() {
  delay(5000);
  Serial.printf("C1: Red leader standing by...\n");
}

void loop1() {
  //Serial.printf("C1: Stay on target...\n");
  delay(500);
}
