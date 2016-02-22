#pragma once 

#include "Buffered.h"

class Sparkles : public Buffered {
public:
  void setup() override;
  void render() override;

private:
	int _frameIdx;
  int _next;
};
