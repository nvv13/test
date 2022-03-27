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



// The normal, core0 setup
void setup() {
  Serial.begin(115200);
  delay(500);

  // This initializes the NeoPixel with RED
#ifdef NEOPIXEL_POWER
  pinMode(NEOPIXEL_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_POWER, NEOPIXEL_POWER_ON);
#endif

  leds.begin();
  leds.setBrightness(10);
  leds.fill(0xff0000);
  leds.show();
    
}

int i_step=0;
 
void loop() {
  Serial.printf("C0: Blue leader standing by...\n");
  delay(1000);
  switch(i_step)
  {
    case 0 :
    {
    leds.fill(0xff0000);
    i_step++;
    };break;
    case 1 :
    {
    leds.fill(0x00ff00);
    i_step++;
    };break;
    case 2 :
    {
    leds.fill(0x0000ff);
    i_step=0;
    };break;
  }
  leds.show();
}

// Running on core1
void setup1() {
  delay(5000);
  Serial.printf("C1: Red leader standing by...\n");
}

void loop1() {
  Serial.printf("C1: Stay on target...\n");
  delay(500);
}
