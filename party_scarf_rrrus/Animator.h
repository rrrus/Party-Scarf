#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "function_lite.h"

extern uint32_t sCurrentFrameTime;
void SetAnimatorCurrentFrameTime(uint32_t time);

template<class ValueType>
class Animator {
public:
  typedef function<void(Animator&)> OnIdle;

  Animator() {
    _startVal = _nextVal = 0;
    _startTime = 0;
    _animEndTime = 1;
    _holdEndTime = 2;
  }

  void setOnIdle(const OnIdle &onIdle) {
    _onIdle = onIdle;
    if (sCurrentFrameTime >= _holdEndTime) {
      _onIdle(*this);
    }
  }

  ValueType value() {
    if (_valueTime != sCurrentFrameTime && _valueTime < _animEndTime) {
      _curVal = interpolateValue();
      _valueTime = sCurrentFrameTime;
    }
    if (sCurrentFrameTime > _holdEndTime) {
      _onIdle(*this);
    }
    return _curVal;
  }

  void set(const ValueType &toValue) {
    _startVal = _nextVal = toValue;
    _startTime = _animEndTime = _holdEndTime = sCurrentFrameTime;
  }

  void animate(const ValueType &toValue, uint32_t duration, uint32_t thenHoldFor = 0) {
    if (duration == 0) {
      _startVal = toValue;
    } else {
      _startVal = interpolateValue();
    }
    _startTime = sCurrentFrameTime;
    _nextVal = toValue;
    _animEndTime = _startTime + duration;
    _holdEndTime = _animEndTime + thenHoldFor;
  }

  ValueType interpolateValue();

private:
  ValueType _startVal, _nextVal, _curVal;
  uint32_t _startTime, _animEndTime, _holdEndTime, _valueTime;
  OnIdle _onIdle;
};

typedef Animator<float> Animatorf;
typedef Animator<CRGB> AnimatorRGB;
