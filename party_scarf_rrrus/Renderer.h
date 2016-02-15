
class Renderer {
public:
  void init(uint16_t num_leds, CRGB *leds, uint8_t *gamma_lut) {
    _num_leds = num_leds;
    _leds = leds;
    _gamma_lut = gamma_lut;
  }
  virtual void setup() = 0;
  virtual void loop() = 0;

protected:
  uint16_t _num_leds;
  CRGB *_leds;
  uint8_t *_gamma_lut;
};