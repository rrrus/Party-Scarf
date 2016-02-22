#include "Animator.h"
#include "Renderer.h"

class ColorFade : public Renderer {
public:
  void setup() override;
  void render() override;

private:
  AnimatorRGB _color;
};
