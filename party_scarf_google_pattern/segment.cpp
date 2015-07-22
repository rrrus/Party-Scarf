#include "Arduino.h"
#include "segment.h"

CRGBPalette16 googleColors = CRGBPalette16(
    0x0266C8, 0xF90101, 0xF2B50F, 0x00933B,
    0x0266C8, 0xF90101, 0xF2B50F, 0x00933B,
    0x0266C8, 0xF90101, 0xF2B50F, 0x00933B,
    0x0266C8, 0xF90101, 0xF2B50F, 0x00933B
  );

Segment::Segment() {
};

void Segment::configure(float speedHead, float speedTail, float start, float end, float dither, CRGB color)
{
  Serial.print("Segment configured with ");
  Serial.print(speedHead);
  Serial.print(",");
  Serial.print(speedTail);
  Serial.print(",");
  Serial.print(start);
  Serial.print(",");
  Serial.print(end);
  Serial.print(",");
  Serial.print(dither);
  Serial.print(",");
  Serial.println(color);
  this->speedHead = speedHead;
  this->speedTail = speedTail;
  this->start = start;
  this->end = end;
  this->dither = dither;
  this->color = color;
};

void Segment::tick(int max) {
  //Serial.println("Ticking");
  start+=speedHead;
  end+=speedTail;
  if (start > max || end > max) {
    this->randomize();
    return;
  }

  dither = start - int(start);
  if ((start + speedHead) == start){

  }
};

void Segment::randomize() {
  this->configure(
    0.1 + (1 * (random8(1000) / 1000.0)),
    0.1 + (1 * (random8(1000) / 1000.0)),
    0,
    0,
    0,
    googleColors[random8(16)]);
}

