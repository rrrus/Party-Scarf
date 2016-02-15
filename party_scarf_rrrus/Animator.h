#ifndef Animator_h
#define Animator_h

#include <Arduino.h>
#include <FastLED.h>

static uint32_t sCurrentFrameTime;
void SetAnimatorCurrentFrameTime(uint32_t time);

template<typename ValueType>
class Animator {
public:
  typedef void(*OnIdle)(Animator&);

  Animator() {
    _startVal = _nextVal = 0;
    _startTime = 0;
    _animEndTime = 1;
    _holdEndTime = 2;
  }

  void setOnIdle(OnIdle onIdle) {
    _onIdle = onIdle;
    if (sCurrentFrameTime >= _holdEndTime) {
      _onIdle(*this);
    }
  }

  ValueType value() {
    ValueType v = valueInternal();
    if (sCurrentFrameTime > _holdEndTime) {
      _onIdle(*this);
    }
    return v;
  }

  void set(ValueType toValue) {
    _startVal = _nextVal = toValue;
    _startTime = _animEndTime = _holdEndTime = sCurrentFrameTime;
  }

  void animate(ValueType toValue, uint32_t duration, uint32_t thenHoldFor = 0) {
    if (duration == 0) {
      _startVal = toValue;
    } else {
      _startVal = valueInternal();
    }
    _startTime = sCurrentFrameTime;
    _nextVal = toValue;
    _animEndTime = _startTime + duration;
    _holdEndTime = _animEndTime + thenHoldFor;
  }

  ValueType valueInternal();

  ValueType _startVal, _nextVal, _curVal;
  uint32_t _startTime, _animEndTime, _holdEndTime, _valueTime;
  OnIdle _onIdle;
};

typedef Animator<float> Animatorf;
typedef Animator<CRGB> AnimatorRGB;

#endif // Animator_h
