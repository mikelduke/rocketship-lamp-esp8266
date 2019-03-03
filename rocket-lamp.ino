#include <EEPROM.h>

#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <FastLED.h>

#define NUM_LEDS 16
#define DATA_PIN 3
#define CLOCK_PIN 13

#define DELAY 40

CRGB leds[NUM_LEDS];

#include "HttpServer.h"
#include "Patterns.h"

/******************* FUNCTIONS **********************/

void setup() {
  Serial.begin(115200);
  Serial.println("Rocket Lamp");

  EEPROM.begin(512);
  loadSettings();

  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(84);

  setupHttpServer();

  // setPattern(1); // force override eeprom
}

void loop() {
  server.handleClient();
  patterns[currentPatternIndex].pattern();
}

void loadSettings() {
  currentPatternIndex = EEPROM.read(0);
  if (currentPatternIndex < 0)
    currentPatternIndex = 0;
  else if (currentPatternIndex >= patternCount)
    currentPatternIndex = patternCount - 1;

  Serial.printf("loadSettings currentPatternIndex:%d\n", currentPatternIndex);
}
