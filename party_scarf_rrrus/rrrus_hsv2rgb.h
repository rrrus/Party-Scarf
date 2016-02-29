#pragma once

#include <Arduino.h>
#include <FastLED.h>

namespace rrrus {

// This hsv2rgb differs from the versions in FastLED in that for
// all possible HSV values, at least one channel of the output
// RGB is always at full v.
CRGB hsv2rgb(const CHSV& hsv, CRGB& rgb);

}
