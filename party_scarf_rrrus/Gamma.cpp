#include <Arduino.h>
#include <FastLED.h>

#include "Gamma.h"

void Gamma::initPower(const float p) {
  for (int i=0; i<256; i++) {
    // Regular gamma function, gamma = 2.8
    _lut[i] = powf((float)i/255, p) * 255 + 0.5;
  }
}

void Gamma::initAntiLog(const float p) {
  const float scaleBack = powf(2, p) - 1;
  for (int i=0; i<256; i++) {
    // rrrus's cool-mo-dee anti-log function, turns on sooner,
    // wider range of mid-tones.
    _lut[i] = ((powf(2, p * (float)i/255) - 1) / scaleBack) * 255 + 0.5;
  }
}
