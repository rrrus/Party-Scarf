#include "Buffered.h"
#include "Globals.h"

CRGB gBufLeds[NUM_LEDS];

void Buffered::setup() {
}

void Buffered::render() {
  for (int i=0; i<NUM_LEDS; i++) {
    // gLeds[i] = gLumaLut[gBufLeds[i]];
    gLeds[i] = gBufLeds[i];

    gBufLeds[i].r = max(0, gBufLeds[i].r - _dim);
    gBufLeds[i].g = max(0, gBufLeds[i].g - _dim);
    gBufLeds[i].b = max(0, gBufLeds[i].b - _dim);
  }
}

