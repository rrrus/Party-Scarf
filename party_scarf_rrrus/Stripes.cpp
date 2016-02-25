#include "fixedp.h"
#include "function_lite.h"
#include "Globals.h"
#include "hsv2rgb_rrrus.h"
#include "rrrandom.h"
#include "Stripes.h"

// 0 bits int, 8 bits fraction.
typedef fixedp<false, 0, 8> ufixdot8;

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
      ufixdot8 s = (ufixdot8::ValType)randi(255);
      // Cube s to make it tend more towards 0, which is then (1-s),
      // so that we end up with more saturation than not.
      s = s * s * s;
      hsv2rgb_rrrus(CHSV(randi(255), (255 - s.fract()), 255), color);
    }
    anim.animate(color,
                 randiRange(1, 10)*SECS,
                 randiRange(10, 30)*SECS);
  };
  _color1.setOnIdle(colorOnIdle);
  _color2.setOnIdle(colorOnIdle);
}

void Stripes::render() {
  const uint32_t now = millis();
  const float deltaTime = (float)(now - _lastTime)/1000.0f;
  _lastTime = now;

  const float stripeWidth = _widthAnim.value();
  _fpos += (_speedAnim.value() * deltaTime);
  if (stripeWidth > 0) {
    while (_fpos > stripeWidth) {
      _fpos -= stripeWidth*2;
    }
    while (_fpos < -stripeWidth) {
      _fpos += stripeWidth*2;
    }
  }

  const float crossover = min(_crossoverAnim.value(), stripeWidth/2);
  const CRGB c1 = _color1.value();
  const CRGB c2 = _color2.value();

  static const int midStrip = NUM_LEDS / 2;
  for (int i=0; i<NUM_LEDS; i++) {
    const float ploc = fabs(fmod(_fpos + i - midStrip + stripeWidth*1000.5, stripeWidth*2) - stripeWidth) - stripeWidth/2;
    const float interp = min(0.5, max(-0.5, ploc / crossover)) + 0.5;
    gLeds[i] = c1.lerp8(c2, 255 * interp);
    gLeds[i].r = gLumaLut[gLeds[i].r];
    gLeds[i].g = gLumaLut[gLeds[i].g];
    gLeds[i].b = gLumaLut[gLeds[i].b];
  }
}

