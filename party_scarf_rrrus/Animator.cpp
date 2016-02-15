
#include "Animator.h"

void SetAnimatorCurrentFrameTime(uint32_t time) {
  sCurrentFrameTime = time;
}

template<>
float Animator<float>::valueInternal() {
  if (_valueTime != sCurrentFrameTime) {
    float interp = 1;
    if (_animEndTime != _startTime) {
      // linear interpolation
      interp = min(1, ((float)sCurrentFrameTime-_startTime)/((float)_animEndTime-_startTime));
    }
    _curVal = _nextVal * interp + _startVal * (1 - interp);
    _valueTime = sCurrentFrameTime;
  }
  return _curVal;
}

template<>
CRGB Animator<CRGB>::valueInternal() {
  if (_valueTime != sCurrentFrameTime) {
    float interp = 1;
    if (_animEndTime != _startTime) {
      // linear interpolation
      interp = min(1, ((float)sCurrentFrameTime-_startTime)/((float)_animEndTime-_startTime));
    }
    _curVal = _startVal.lerp16(_nextVal, 65535 * interp);
    _valueTime = sCurrentFrameTime;
  }
  return _curVal;
}

