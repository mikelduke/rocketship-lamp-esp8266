#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include <FastLED.h>
#include <EEPROM.h>

#define NUM_LEDS 16
#define DATA_PIN 3
#define CLOCK_PIN 13

#define DELAY 40

// Define the array of leds
CRGB leds[NUM_LEDS];

uint8_t currentPatternIndex = 0;

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

typedef void (*Pattern)();
typedef Pattern PatternList[];
typedef struct {
  Pattern pattern;
  String name;
} PatternAndName;
typedef PatternAndName PatternAndNameList[];

#include "Patterns.h"

PatternAndNameList patterns = {
  { ring,                  "Ring" },
  { ring_two,              "Ring Type 2" },
  { huefade,               "Hue Fade" }
};

const uint8_t patternCount = ARRAY_SIZE(patterns);

void setup() {
  Serial.begin(115200);
  Serial.println("Rocket Lamp");

  EEPROM.begin(512);
  loadSettings();

  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(84);

  // setPattern(1); // force override eeprom
}

void loop() {
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

void setPattern(uint8_t value) {
  if (value >= patternCount)
    value = patternCount - 1;

  currentPatternIndex = value;

  EEPROM.write(0, currentPatternIndex);
  EEPROM.commit();

  Serial.printf("Pattern: %d\n", currentPatternIndex);
}