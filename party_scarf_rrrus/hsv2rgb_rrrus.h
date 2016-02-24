#pragma once

#include <Arduino.h>
#include <FastLED.h>

// This hsv2rgb differs from the versions in FastLED in that for
// all possible HSV values, at least one channel of the output
// RGB is always at full v.
CRGB hsv2rgb_rrrus(const CHSV& hsv, CRGB& rgb);
