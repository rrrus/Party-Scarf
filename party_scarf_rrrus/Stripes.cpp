#include "Stripes.h"
#include "Globals.h"
#include "rrrandom.h"

void Stripes::setup() {
  _chaseAnim.set(NUM_LEDS/2);
  _chaseAnim.setOnIdle([](Animatorf &anim) {
    anim.animate(randfRange(-10, 10), randi(5*SECS), randi(30*SECS));
  });

  _widthAnim.set(5);
  _widthAnim.setOnIdle([](Animatorf &anim) {
    // float width = randf(0.5, )
    anim.animate(randfRange(2, NUM_LEDS), randiRange(5, 10)*SECS, randiRange(10, 20)*SECS);
  });

  _crossoverAnim.set(1);
  _crossoverAnim.setOnIdle([](Animatorf &anim) {
    float next = randfRange(0.5, NUM_LEDS/2);
    anim.animate(next, randi(5*SECS), randiRange(10, 30)*SECS);
    // Serial.print("next crossover ");
    // Serial.println(next);
  });

  _color1.set(CRGB::Black);
  _color2.set(CRGB::Black);
  AnimatorRGB::OnIdle colorOnIdle = [](AnimatorRGB &anim) {
    float s = randf(1);
    s = s*s*s;
    anim.animate(CHSV(randi(255), 255, 255), randi(10*SECS), randiRange(10, 30)*SECS);
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
    float interp = min(0.5, max(-0.5, ploc / crossover)) + 0.5;
    gLeds[i] = c1.lerp8(c2, 255 * interp);
    // gLeds[i].r = gLumaLut[gLeds[i].r];
    // gLeds[i].g = gLumaLut[gLeds[i].g];
    // gLeds[i].b = gLumaLut[gLeds[i].b];
  }
}

