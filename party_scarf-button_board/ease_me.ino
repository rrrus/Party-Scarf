void ease_me() {
  EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    easeMe();
  }
  show_at_max_brightness_for_power();                         // Keep this OUTSIDE the loop.
} // loop()

uint8_t lastLerp = 0;

void easeMe() {

  static uint8_t easeOutVal = 0;
  static uint8_t easeInVal  = 0;
  static uint8_t lerpVal    = 0;

  easeOutVal = ease8InOutQuad(easeInVal);
  easeInVal++;

  lerpVal = lerp8by8(0, NUM_LEDS, easeOutVal);

  leds[lerpVal] = CRGB::Red;
  fadeToBlackBy(leds, NUM_LEDS, 16);                     // 8 bit, 1 = slow, 255 = fast
} // loop()