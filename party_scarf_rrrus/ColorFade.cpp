#include "ColorFade.h"
#include "Globals.h"
#include "rrrandom.h"

void ColorFade::setup() {
  _color.set(CRGB::Black);
  _color.setOnIdle([this](AnimatorRGB &anim) {
    CRGB toColor = CRGB::White;
    if (_fadeToWhite) {
      toColor = CHSV(randi(255), randi(255), 255);
    }
    _fadeToWhite = !_fadeToWhite;
    anim.animate(toColor, randi(5_s), randiRange(1_s, 5_s));
  });
}

void ColorFade::render() {
  fill_solid(gLeds, NUM_LEDS, _color.value());
}

