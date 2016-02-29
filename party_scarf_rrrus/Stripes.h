#pragma once

#include "Animator.h"
#include "Renderer.h"

class Stripes : public Renderer {
public:
  void setup() override;
  void render() override;

private:
  Animatorf _speedAnim;
  Animatorf _widthAnim;
  Animatorf _crossoverAnim;
  AnimatorRGB _color1;
  AnimatorRGB _color2;
  uint32_t _lastTime{0};
  float _fpos{0};
};
