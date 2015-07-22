
#ifndef Segment_h
#define Segment_h

#include "Arduino.h"
#include <FastLED.h>

class Segment
{
  public:
    Segment();
    void configure(float speedHead, float speedTail, float start, float end, float dither, CRGB color);
    void tick(int max);
    void randomize();

    float speedHead;
    float speedTail;
    float start;
    float end;
    float dither;
    CRGB color;

};

#endif
