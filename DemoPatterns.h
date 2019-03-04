#ifndef _DEMO_PATTERNS_H
#define _DEMO_PATTERNS_H

void rainbow() {
  static uint8_t hue = 0;
  
  fill_rainbow( leds, NUM_LEDS, hue++, 7);
  FastLED.show();

  delay(DELAY / 2);
}

void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void rainbowWithGlitter() {
  static uint8_t hue = 0;
  
  fill_rainbow( leds, NUM_LEDS, hue++, 7);
  addGlitter(80);
  FastLED.show();
  delay(DELAY / 2);
}

void confetti() {
  // random colored speckles that blink in and fade smoothly
  static uint8_t hue = 0;
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( hue, 200, 255);
  hue += random8(64);
  FastLED.show();

  delay(DELAY / 2);
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
  FastLED.show();
  delay(DELAY / 2);
}

#endif
