void fadeall() { 
  for(int i = 0; i < NUM_LEDS; i++) { 
    leds[i].nscale8(250); 
  } 
}

void ring() {
  static uint8_t hue = 0;

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

  for(int i = 0; i < NUM_LEDS / 2; i++) {
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

  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue, 255, 255);
  }
  hue++;
  FastLED.show(); 
  delay(DELAY / 2);
}
