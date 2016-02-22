#include "ColorFade.h"
#include "Globals.h"
#include "rrrandom.h"

static bool gFadeToWhite = false;

void ColorFade::setup() {
  _color.set(CRGB::Black);
  AnimatorRGB::OnIdle colorOnIdle = [](AnimatorRGB &anim) {
    CRGB toColor = CRGB::White;
    if (true || !gFadeToWhite) {
      toColor = CHSV(randi(255), randi(255), 255);
    }
    gFadeToWhite = !gFadeToWhite;
    anim.animate(toColor, randi(5*SECS), randiRange(1, 5)*SECS);
  };
  _color.setOnIdle(colorOnIdle);
}

void ColorFade::render() {
  fill_solid(gLeds, NUM_LEDS, _color.value());
}

