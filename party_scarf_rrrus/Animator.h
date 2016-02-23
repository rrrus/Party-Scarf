#pragma once

#include <Arduino.h>
#include <FastLED.h>

extern uint32_t sCurrentFrameTime;
void SetAnimatorCurrentFrameTime(uint32_t time);

template<typename ValueType>
class Animator {
public:
  class IOnIdle2 {
    public:
      virtual void execute(Animator&) = 0;
  };

  typedef void(*OnIdle)(Animator&);

  Animator() {
    _startVal = _nextVal = 0;
    _startTime = 0;
    _animEndTime = 1;
    _holdEndTime = 2;
  }

  void setOnIdle(OnIdle onIdle) {
    _onIdle = onIdle;
    if (_onIdle && sCurrentFrameTime >= _holdEndTime) {
      _onIdle(*this);
    }
  }

  void setOnIdle2(IOnIdle2 *onIdle2) {
    _onIdle2 = onIdle2;
    if (_onIdle2 && sCurrentFrameTime >= _holdEndTime) {
      _onIdle2->execute(*this);
    }
  }

  ValueType value() {
    if (_valueTime != sCurrentFrameTime && _valueTime < _animEndTime) {
      _curVal = interpolateValue();
      _valueTime = sCurrentFrameTime;
    }
    if (sCurrentFrameTime > _holdEndTime) {
      if (_onIdle) {
        _onIdle(*this);
      } else if (_onIdle2) {
        _onIdle2->execute(*this);
      }
    }
    return _curVal;
  }

  void set(ValueType toValue) {
    _startVal = _nextVal = toValue;
    _startTime = _animEndTime = _holdEndTime = sCurrentFrameTime;
  }

  void animate(ValueType toValue, uint32_t duration, uint32_t thenHoldFor = 0) {
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

  template<typename Func>
  class OnIdle2 : public IOnIdle2 {
    public:
      Func funcData;

      OnIdle2(Func &func) : funcData(func) {}

      void execute(Animator& a){
        funcData(a);
      }
  };

private:
  ValueType _startVal, _nextVal, _curVal;
  uint32_t _startTime, _animEndTime, _holdEndTime, _valueTime;
  OnIdle _onIdle;
  IOnIdle2 *_onIdle2;
};

typedef Animator<float> Animatorf;
typedef Animator<CRGB> AnimatorRGB;
