#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "Gamma.h"

const uint16_t NUM_LEDS = 48;
extern CRGB gLeds[];
extern Gamma gGamma;

// This handy operator lets you specify seconds literals in milliseconds
// with the '_s' suffix.  e.g. 5_s == 5000
constexpr unsigned long long int operator "" _s(unsigned long long int d) {
  return d * 1000;
}
constexpr long double operator "" _s(long double d) {
  return d * 1000;
}
