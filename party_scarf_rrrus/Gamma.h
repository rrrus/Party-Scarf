#pragma once

class Gamma {
public:
  // Inits the brightness curve with a traditional gamma curve of N^p.
  void initPower(float p = 2.6f);

  // Inits the brightness curve with an anti log curve of 2^Np - 1.
  void initAntiLog(float p = 6);

  inline CRGB apply(const CRGB &c) {
    return CRGB(_lut[c.r], _lut[c.g], _lut[c.b]);
  }

  inline void apply(const CRGB &in, CRGB &out) {
    out.r = _lut[in.r];
    out.g = _lut[in.g];
    out.b = _lut[in.b];
  }

private:
  uint8_t _lut[256];
};
