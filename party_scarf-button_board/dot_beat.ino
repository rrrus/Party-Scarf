// Define variables used by the sequences.
int   thisdelay =   10;                                        // A delay value for the sequence(s)
uint8_t   count =   0;                                        // Count up to 255 and then reverts to 0
uint8_t fadeval = 20;  

void dot_beat() {

  uint8_t inner = beatsin8(bpmAvg/4, NUM_LEDS/4, NUM_LEDS/4*3);
  uint8_t outer = beatsin8(bpmAvg/4, 0, NUM_LEDS-1);
  uint8_t middle = beatsin8(bpmAvg/4, NUM_LEDS/3, NUM_LEDS/3*2);

  leds[middle] = CRGB::Purple;
  leds[inner] = CRGB::Blue;
  leds[outer] = CRGB::Aqua;

  //nscale8(leds,NUM_LEDS,fadeval);                             // Fade the entire array. Or for just a few LED's, use  nscale8(&leds[2], 5, fadeval);
  fadeToBlackBy(leds, NUM_LEDS, 16);                     // 8 bit, 1 = slow, 255 = fast
} // dot_beat()