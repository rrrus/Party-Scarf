#include "Animator.h"
#include "Chaser.h"
#include "Globals.h"
#include "rrrandom.h"
#include <FastLED.h>

const uint16_t LED_PIN = 4;
const uint16_t CLOCK_PIN = 3;
const EOrder COLOR_ORDER = BGR;
const ESPIChipsets CHIPSET = APA102;
const uint16_t NUM_LEDS = 48;

const uint8_t BRIGHTNESS = 100;

CRGB gLeds[NUM_LEDS];
uint8_t gLumaLut[256];

Chaser chaser;

void setup() {
  //delay(3000); // sanity delay
  randomSeed(analogRead(0));

  //setup serial communications through the USB
  Serial.begin(9600);
  //Let's print a start messgae to the serial monitor when a new sequence of random numbers starts
  Serial.println("Starting new Random Number Sequence");

  for (int i=0; i<256; i++) {
    // Regular gamma function, gamma = 2.8
    // gLumaLut[i] = powf((float)i/255, 2.8) * 255 + 0.5;
    // rrrus's cool-mo-dee anti-log function
    gLumaLut[i] = (powf(2, 8 * (float)i/255) / 256) * 255 + 0.5;
  }
  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER>(gLeds, NUM_LEDS).setCorrection( UncorrectedColor );
  FastLED.setBrightness( BRIGHTNESS );

  SetAnimatorCurrentFrameTime(millis());

  chaser.setup();
}

void loop() {
  SetAnimatorCurrentFrameTime(millis());
  chaser.loop();
  FastLED.show();
  delay(7);
}

