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
};
