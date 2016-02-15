
#include "Animator.h"

uint32_t sCurrentFrameTime;

void SetAnimatorCurrentFrameTime(uint32_t time) {
  sCurrentFrameTime = time;
}

template<>
float Animator<float>::interpolateValue() {
  float interp = 1;
  if (_animEndTime != _startTime) {
    // linear interpolation
    interp = min(1, ((float)sCurrentFrameTime-_startTime)/((float)_animEndTime-_startTime));
  }
  return _nextVal * interp + _startVal * (1 - interp);
}

template<>
CRGB Animator<CRGB>::interpolateValue() {
  uint32_t interp = 0xffff;
  if (_animEndTime != _startTime) {
    // linear interpolation
    interp = min(0xffff, (sCurrentFrameTime-_startTime)*0xffff/(_animEndTime-_startTime));
  }
  return _startVal.lerp16(_nextVal, interp);
}

