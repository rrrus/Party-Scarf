#include <FastLED.h>

#define LED_PIN     3
#define CLOCK_PIN   4
#define COLOR_ORDER GBR
#define CHIPSET     APA102
#define NUM_LEDS    48

#define BRIGHTNESS  120
#define FRAMES_PER_SECOND 200

CRGB leds[NUM_LEDS];
uint8_t wheelpos = 0;
unsigned long time;
unsigned long last_time;

#define NUM_PATTERNS 2

void fast_rainbow() {
  fill_rainbow(leds, NUM_LEDS, wheelpos, 2);
  wheelpos++;
  FastLED.delay(1000 / 200);
}

void slow_rainbow() {
  fill_rainbow(leds, NUM_LEDS, wheelpos, 2);
  wheelpos++;
  FastLED.delay(1000 / 30);
}


void (*p[NUM_PATTERNS]) ();
int pn = 0;

void setup() {
  //delay(3000); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  p[0] = fast_rainbow;
  p[1] = slow_rainbow;
  last_time = millis();
}

void loop()
{
  time = millis();
  // Add entropy to random number generator; we use a lot of it.
  // random16_add_entropy( random());
  if (((time - last_time) / 1000) > 3) {
    pn++;
    if (pn > NUM_PATTERNS) {
      pn = 0;
    }
  }
  (*p[pn]) ();
  //rainbow(); // run simulation frame
  FastLED.show(); // display this frame  
}

