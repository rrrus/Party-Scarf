#include "fixedp.h"
#include "Globals.h"
#include "rrrandom.h"
#include "rrrus_function.h"
#include "rrrus_hsv2rgb.h"
#include "Stripes.h"

// 0 bits int, 8 bits fraction.
typedef fixedp<false, 0, 8> ufixdot8;

void Stripes::setup() {
  // Start at 0 speed, hold for 5 seconds.
  _speedAnim.animate(0, 0, 5_s);
  _speedAnim.setOnIdle([](Animatorf &anim) {
    anim.animate(randfRange(-15, 15),
                 randiRange(0.5_s, 5_s),
                 randiRange(1_s, 30_s));
  });

  _widthAnim.animate(3, 0, 10_s);
  _widthAnim.setOnIdle([](Animatorf &anim) {
    // Have to sqrt the min range since so that squaring it later gives
    // the right min range val.
    constexpr float minRange = sqrtf(4.0f / NUM_LEDS);
    float width = randfRange(minRange, 1);
    width = width * width;
    anim.animate(width * NUM_LEDS,
                 randiRange(5_s, 10_s),
                 randiRange(10_s, 20_s));
  });

  _crossoverAnim.animate(2, 0, 15_s);
  _crossoverAnim.setOnIdle([](Animatorf &anim) {
    // Min crossover of 2 leds gives enough blur to hide the discrete lights.
    anim.animate(randfRange(2, NUM_LEDS/2),
                 randiRange(1_s, 5_s),
                 randiRange(10_s, 30_s));
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
      rrrus::hsv2rgb(CHSV(randi(255), (255 - s.fract()), 255), color);
    }
    anim.animate(color,
                 randiRange(1_s, 10_s),
                 randiRange(10_s, 30_s));
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

  const float halfWidth = stripeWidth/2;
  const float crossover = min(_crossoverAnim.value(), halfWidth);
  const CRGB c1 = _color1.value();
  const CRGB c2 = _color2.value();

  constexpr auto midStrip = NUM_LEDS / 2;
  for (int i=0; i<NUM_LEDS; i++) {
    const float ploc = fabs(fmod(_fpos + i - midStrip + stripeWidth*1000.5, stripeWidth*2) - stripeWidth) - halfWidth;
    const float interp = min(0.5, max(-0.5, ploc / crossover)) + 0.5;
    gLeds[i] = gGamma.apply( c1.lerp8(c2, 255 * interp));
  }
}

