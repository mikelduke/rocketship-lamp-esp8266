#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <FastLED.h>

#define NUM_LEDS 16
#define DATA_PIN 3
#define CLOCK_PIN 13

#define DELAY 40

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
  Serial.begin(115200);
  Serial.println("Rocket Lamp");
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(84);
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void loop() { 
  huefade();
}

void ring() {
  static uint8_t hue = 0;
  Serial.println("ring");

  for(int i = 0; i < NUM_LEDS / 2; i++) {
    leds[i] = CHSV(hue++, 255, 255);
    leds[i + (NUM_LEDS /2)] = CHSV(hue++, 255, 255);
    FastLED.show(); 
    
    fadeall();
    delay(DELAY);
  }
}

void ring_two() {
  static uint8_t hue = 0;
  Serial.println("ring_two");

  for(int i = 0; i < NUM_LEDS / 2; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    leds[i + (NUM_LEDS /2)] = CHSV(hue++, 255, 255);
    FastLED.show(); 

    leds[i] = CRGB::Black;
    leds[i + (NUM_LEDS /2)] = CRGB::Black;

    delay(DELAY);
  }
}

void huefade() {
  static uint8_t hue = 0;
  Serial.println("huefade");

  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, 255, 255);
  }
  hue++;
  FastLED.show(); 
  delay(DELAY / 2);
}