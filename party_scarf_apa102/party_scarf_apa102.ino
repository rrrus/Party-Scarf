#include <FastLED.h>

#define LED_PIN     3
#define CLOCK_PIN   4
#define COLOR_ORDER GBR
#define CHIPSET     APA102
#define NUM_LEDS    48

#define BRIGHTNESS  100
#define FRAMES_PER_SECOND 20

CRGB leds[NUM_LEDS];
uint8_t wheelpos = 0;

void setup() {
  //delay(3000); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
}

void loop()
{
  rainbow(); // run simulation frame

  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void rainbow() {
  fill_rainbow(leds, NUM_LEDS, wheelpos, 2);
  wheelpos++;
}

