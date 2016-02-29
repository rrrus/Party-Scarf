#include "Animator.h"
#include "ColorFade.h"
#include "Globals.h"
#include "rrrandom.h"
#include "Sparkles.h"
#include "Stripes.h"

const uint16_t LED_PIN = 4;
const uint16_t CLOCK_PIN = 3;
const EOrder COLOR_ORDER = BGR;
const ESPIChipsets CHIPSET = APA102;

const uint8_t BRIGHTNESS = 100;
const uint32_t FRAMES_PER_SECOND = 60;

CRGB gLeds[NUM_LEDS];
Gamma gGamma;

static const uint32_t NFRAMETIMES = 20;
uint32_t gLastFrameTime = 0;
uint32_t gCurrentFrame = 0;
int32_t gFrameDelay = 10;
int32_t gLastFPS = 0;

Renderer *gRenderer = nullptr;

void setup() {
  //delay(3000); // sanity delay

  randomSeed(analogRead(0));

  //setup serial communications through the USB
  Serial.begin(9600);
  Serial.println("HOW Y'ALL DOIN?");

  // Init FastLED.
  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER>(gLeds, NUM_LEDS).setCorrection( UncorrectedColor );
  FastLED.setBrightness( BRIGHTNESS );

  // Initialize the gamma LUT.
  gGamma.initAntiLog(6);

  // Initialize Animator with a start time.
  SetAnimatorCurrentFrameTime(millis());

  gRenderer =
    // new ColorFade();
    new Stripes();
    // new Sparkles();

  gRenderer->setup();
}

void loop() {
  const uint32_t now = millis();
  SetAnimatorCurrentFrameTime(now);

  // Track frame times/FPS.  Dynamically adjust the per-frame delay to maintain
  // desired FPS.
  if (gCurrentFrame == 0) {
    if (gLastFrameTime != 0) {
      const int32_t fps = NFRAMETIMES * 1000 / (now - gLastFrameTime);
      if (abs(fps - gLastFPS) > 1) {
        // Serial.print("FPS ");
        // Serial.println(fps);
        gLastFPS = fps;
      }
      const int32_t mspf = (now - gLastFrameTime) / NFRAMETIMES;
      const int32_t targetMspf = 1000 / FRAMES_PER_SECOND;
      const int32_t diff = mspf - targetMspf;
      // Little bit of hysteresis on adjusting frame delay. Higher threshold for
      // decreasing frame delay than increasing.
      if (diff < 0 || diff > 1) {
        gFrameDelay = max(0, gFrameDelay - diff);
        // Serial.print("Adjusting frame delay to ");
        // Serial.println(gFrameDelay);
      }
    }
    gLastFrameTime = now;
  }
  gCurrentFrame = (gCurrentFrame + 1) % NFRAMETIMES;

  gRenderer->render();

  FastLED.show();
  delay(gFrameDelay);
}

