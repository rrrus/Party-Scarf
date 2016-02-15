#include "Chaser.h"
#include "Globals.h"
#include "rrrandom.h"

void Chaser::setup() {
  _chaseAnim.set(NUM_LEDS/2);
  _chaseAnim.setOnIdle([](Animatorf &anim) {
    anim.animate(randfRange(-10, 10), random(5*1000), random(30*1000));
  });

  _widthAnim.set(5);
  _widthAnim.setOnIdle([](Animatorf &anim) {
    anim.animate(randf(NUM_LEDS), random(5*1000)+5000, random(10*1000)+10000);
  });

  _crossoverAnim.set(1);
  _crossoverAnim.setOnIdle([](Animatorf &anim) {
    anim.animate(randfRange(0.5, NUM_LEDS/4), random(5*1000), random(10*10000)+20000);
  });

  _color1.set(CRGB::Black);
  _color2.set(CRGB::Black);
  AnimatorRGB::OnIdle colorOnIdle = [](AnimatorRGB &anim) {
    float s = randf(1);
    s = s*s*s;
    anim.animate(CHSV(random(255), 255, 255), random(10*1000), random(30*1000));
  };
  _color1.setOnIdle(colorOnIdle);
  _color2.setOnIdle(colorOnIdle);
}

void Chaser::loop() {
  static uint32_t lastTime = 0;
  const uint32_t now = millis();
  const float deltaTime = (float)(now - lastTime)/1000.0f;
  lastTime = now;

  const CRGB c1 = _color1.value();
  const CRGB c2 = _color2.value();
  const float tailLength = _widthAnim.value();
  static float fpos;
  fpos += (_chaseAnim.value() * deltaTime);
  while (fpos > NUM_LEDS) {
    fpos -= tailLength*2;
  }
  while (fpos < 0) {
    fpos += tailLength*2;
  }
  const float crossover = min(_crossoverAnim.value(), tailLength/2);
  for (int i=0; i<NUM_LEDS; i++) {
    float ploc = fabs(fmod(fpos + i, tailLength*2) - tailLength) - tailLength/2;
    float interp = min(0.5*crossover, max(-0.5*crossover, ploc)) / crossover + 0.5;
    gLeds[i] = c1.lerp8(c2, 255 * interp);
    // gLeds[i].r = gLumaLut[gLeds[i].r];
    // gLeds[i].g = gLumaLut[gLeds[i].g];
    // gLeds[i].b = gLumaLut[gLeds[i].b];
  }
}

