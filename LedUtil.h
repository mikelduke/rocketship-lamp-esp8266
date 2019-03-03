#ifndef _LED_UTIL_H
#define _LED_UTIL_H

#include <FastLED.h>

void setColor(const CRGB& color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
  FastLED.show();
}

void flashLeds(const CRGB& color, int times, unsigned long delayTimeMs) {
  for (int i = 0; i < times; i++) {
    setColor(color);
    delay(delayTimeMs);
    setColor(CRGB::Black);
    delay(delayTimeMs);
  }
}

void flashLeds(const CRGB& color, int times) {
  flashLeds(color, times, 50);
}

#endif
