#include "Buffered.h"
#include "Globals.h"

CRGB gBufLeds[NUM_LEDS];

void Buffered::setup() {
}

void Buffered::render() {
  for (int i=0; i<NUM_LEDS; i++) {
    gGamma.apply(gBufLeds[i], gLeds[i]);
    // gLeds[i] = gBufLeds[i];

    if (_dim != 0) {
      gBufLeds[i].subtractFromRGB(_dim);
    }
  }
}
