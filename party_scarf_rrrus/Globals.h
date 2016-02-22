#pragma once

#include <Arduino.h>
#include <FastLED.h>

const uint16_t NUM_LEDS = 48;
extern CRGB gLeds[];
extern uint8_t gLumaLut[];

static const int SECS = 1000;
