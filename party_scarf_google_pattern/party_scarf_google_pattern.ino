#include <FastLED.h>
#include "segment.h"

#define LED_PIN     4
#define CLOCK_PIN   3
#define COLOR_ORDER BGR
#define CHIPSET     APA102
#define NUM_LEDS    300

#define BRIGHTNESS  100
#define FRAMES_PER_SECOND 20
#define MAX_SEGMENTS 10

CRGB leds[NUM_LEDS];
uint8_t wheelpos = 0;
Segment segments[MAX_SEGMENTS];
long lastTick;

void setup() {
  delay(1000); // sanity delay
  Serial.begin(9600);
  random16_add_entropy( random());
  lastTick = millis();

  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  CRGBPalette16 googleColors = CRGBPalette16(
    0x0266C8, 0xF90101, 0xF2B50F, 0x00933B,
    0x0266C8, 0xF90101, 0xF2B50F, 0x00933B,
    0x0266C8, 0xF90101, 0xF2B50F, 0x00933B,
    0x0266C8, 0xF90101, 0xF2B50F, 0x00933B
  );

  for(int i = 0; i < MAX_SEGMENTS; i++){
    segments[i].randomize();
  }
  //   float speedHead = 0.1 + (1 * (random8(1000) / 1000.0));
  //   float speedTail = 0.1 + (1 * (random8(1000) / 1000.0));
  //   //int start = random8(NUM_LEDS);
  //   //int end = random8(NUM_LEDS);
  //   int start = 0;
  //   int end = 0;
  //   int colorIdx = random8(16);
  //   CRGB color = googleColors[colorIdx];
  //   segments[i].configure(
  //     speedHead,
  //     speedTail,
  //     start,
  //     end,
  //     0,
  //     color
  //   );
  // }
  Serial.println("Setup is done");
}

void loop()
{
  FastLED.clear();
  random16_add_entropy( random());
  drawSegments(); // run simulation frame
    for (int i = 0; i < MAX_SEGMENTS; i++) {
      segments[i].tick(NUM_LEDS);

  }
  FastLED.show(); // display this frame
  //FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void drawSegments() {

  for (int i = 0; i < MAX_SEGMENTS; i++) {
    Segment* s = &segments[i];
    int start = s->start > s->end ? s->end : s->start;
    int end = s->start > s->end ? s->start : s->end;
    // Serial.print("Segment start: ");
    // Serial.print(start);
    // Serial.print(", segment end: ");
    // Serial.println(end);

    for (int x = start; x < end; x++) {
      if (x > NUM_LEDS) {
        continue;
      }
      if (x == start || x == end) {
        leds[x] = blend(leds[x], s->color, s->dither);
      } else {
        leds[x] = s->color;
      }

    }
  }
}

void rainbow() {
  fill_rainbow(leds, NUM_LEDS, wheelpos, 2);
  wheelpos++;
}

