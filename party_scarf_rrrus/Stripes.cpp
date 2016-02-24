#include "function_lite.h"
#include "Globals.h"
#include "hsv2rgb_rrrus.h"
#include "rrrandom.h"
#include "Stripes.h"

void Stripes::setup() {
  // Start at 0 speed, hold for 5 seconds.
  _speedAnim.animate(0, 0, 5*SECS);
  _speedAnim.setOnIdle([](Animatorf &anim) {
    anim.animate(randfRange(-15, 15),
                 randi(5*SECS),
                 randi(30*SECS));
  });

  _widthAnim.animate(3, 0, 10*SECS);
  _widthAnim.setOnIdle([](Animatorf &anim) {
    // Have to sqrt the min range since so that squaring it later gives
    // the right min range val.
    static const float minRange = sqrtf(4.0f / NUM_LEDS);
    float width = randfRange(minRange, 1);
    width = width * width;
    anim.animate(width * NUM_LEDS,
                 randiRange(5, 10)*SECS,
                 randiRange(10, 20)*SECS);
  });

  _crossoverAnim.animate(2, 0, 15*SECS);
  _crossoverAnim.setOnIdle([](Animatorf &anim) {
    // Min crossover of 2 leds gives enough blur to hide the discrete lights.
    anim.animate(randfRange(2, NUM_LEDS/2),
                 randi(5*SECS),
                 randiRange(10, 30)*SECS);
  });

  _color1.set(CRGB::Black);
  _color2.set(CRGB::Black);
  AnimatorRGB::OnIdle colorOnIdle = [](AnimatorRGB &anim) {
    CRGB color = CRGB::Black;
    // 80% of the time, make a color.  20% of the time, use black.
    if (randi(100) > 20) {
      float s = randf(1);
      // Cube s to make it tend more towards 0.
      s = s*s*s;
      hsv2rgb_rrrus(CHSV(randi(255), (1-s) * 255, 255), color);
    }
    anim.animate(color,
                 randi(10*SECS),
                 randiRange(10, 30)*SECS);
  };
  _color1.setOnIdle(colorOnIdle);
  _color2.setOnIdle(colorOnIdle);
}

void Stripes::render() {
  static uint32_t lastTime = 0;
  const uint32_t now = millis();
  const float deltaTime = (float)(now - lastTime)/1000.0f;
  lastTime = now;

  const CRGB c1 = _color1.value();
  const CRGB c2 = _color2.value();
  const float tailLength = _widthAnim.value();
  static float fpos = 0;
  fpos += (_speedAnim.value() * deltaTime);
  if (tailLength > 0) {
    while (fpos > tailLength) {
      fpos -= tailLength*2;
    }
    while (fpos < -tailLength) {
      fpos += tailLength*2;
    }
  }
  const float crossover = min(_crossoverAnim.value(), tailLength/2);
  for (int i=0; i<NUM_LEDS; i++) {
    static const int midStrip = NUM_LEDS / 2;
    float ploc = fabs(fmod(fpos + i - midStrip + tailLength*1000.5, tailLength*2) - tailLength) - tailLength/2;
    float interp = min(0.5, max(-0.5, ploc / crossover)) + 0.5;
    gLeds[i] = c1.lerp8(c2, 255 * interp);
    gLeds[i].r = gLumaLut[gLeds[i].r];
    gLeds[i].g = gLumaLut[gLeds[i].g];
    gLeds[i].b = gLumaLut[gLeds[i].b];
  }
}

