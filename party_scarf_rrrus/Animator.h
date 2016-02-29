#pragma once

#include <Arduino.h>
#include <FastLED.h>
#include "rrrus_function.h"

namespace _Animator {
  extern uint32_t sCurrentFrameTime;
}

void SetAnimatorCurrentFrameTime(uint32_t time);

template<class ValueType>
class Animator {
public:
  typedef rrrus::function<void(Animator&)> OnIdle;

  void setOnIdle(const OnIdle &onIdle) {
    _onIdle = onIdle;
    if (_Animator::sCurrentFrameTime >= _holdEndTime) {
      _onIdle(*this);
    }
  }

  ValueType value() {
    if (_valueTime != _Animator::sCurrentFrameTime && _valueTime < _animEndTime) {
      _curVal = interpolateValue();
      _valueTime = _Animator::sCurrentFrameTime;
    }
    if (_Animator::sCurrentFrameTime > _holdEndTime) {
      _onIdle(*this);
    }
    return _curVal;
  }

  void set(const ValueType &toValue) {
    _startVal = _nextVal = toValue;
    _startTime = _animEndTime = _holdEndTime = _Animator::sCurrentFrameTime;
  }

  void animate(const ValueType &toValue, uint32_t duration, uint32_t thenHoldFor = 0) {
    if (duration == 0) {
      _startVal = toValue;
    } else {
      _startVal = interpolateValue();
    }
    _startTime = _Animator::sCurrentFrameTime;
    _nextVal = toValue;
    _animEndTime = _startTime + duration;
    _holdEndTime = _animEndTime + thenHoldFor;
  }

  ValueType interpolateValue();

private:
  ValueType _startVal{0}, _nextVal{0}, _curVal{0};
  uint32_t _startTime{0}, _animEndTime{1}, _holdEndTime{2}, _valueTime{2};
  OnIdle _onIdle;
};

typedef Animator<float> Animatorf;
typedef Animator<CRGB> AnimatorRGB;
