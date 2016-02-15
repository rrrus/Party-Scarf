#include "Animator.h"
#include "Renderer.h"

class Chaser : public Renderer {
public:
  void setup() override;
  void render() override;

private:
  Animatorf _chaseAnim;
  Animatorf _widthAnim;
  Animatorf _crossoverAnim;
  AnimatorRGB _color1;
  AnimatorRGB _color2;
};
