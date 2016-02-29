#pragma once

#include <FastLED.h>
#include "Renderer.h"

// Render to this in a Buffered subclass.
extern CRGB gBufLeds[]; // NUM_LEDS

// Base class for renders that want to render to a persistent
// linear buffer that is copied through the luma lut to the
// actual LED buffer at the end of render.  Subclasses should
// call Buffered::render() at the end of their own render()
// method.
class Buffered : public Renderer {
public:
  void setup() override;
  void render() override;

protected:
  int _dim{0};
private:
};
