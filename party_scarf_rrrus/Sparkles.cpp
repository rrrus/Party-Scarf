#include "Sparkles.h"
#include "Globals.h"
#include "rrrandom.h"
#include "rrrus_hsv2rgb.h"

void Sparkles::setup() {
  _dim = 7;
}

void Sparkles::render() {
  // SPARKLE!!
  if (_frameIdx == _next) {
    int idx = randi(NUM_LEDS-1);
    rrrus::hsv2rgb(CHSV(randi(255), randi(64), 255), gBufLeds[idx]);
    _next = _frameIdx + randiRange(3, 30);
  }
  _frameIdx++;
	Buffered::render();
}

