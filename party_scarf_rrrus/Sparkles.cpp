#include "Sparkles.h"
#include "Globals.h"
#include "rrrandom.h"

void Sparkles::setup() {
  _dim = 8;
}

void Sparkles::render() {
  // SPARKLE!!
  if (_frameIdx == _next) {
    int idx = randi(NUM_LEDS);
    gBufLeds[idx] = CHSV(randi(255), random(0,64), 255);
    _next = _frameIdx + randiRange(12, 36);
  }
  _frameIdx++;
	Buffered::render();
}

