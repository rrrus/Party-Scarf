CRGBPalette16 currentPalette = RainbowColors_p;
CRGBPalette16 targetPalette = RainbowColors_p;
uint8_t maxChanges = 24; 
TBlendType    currentBlending = LINEARBLEND;;

void colorpal_beat() {
  uint8_t beatA = beatsin8(5, 0, 255);                        // Starting hue
  FillLEDsFromPaletteColors(beatA);
  
  EVERY_N_MILLISECONDS(100) {                                // FastLED based timer to update/display the sequence every 5 seconds.
    nblendPaletteTowardPalette( currentPalette, targetPalette, maxChanges);
  }

  EVERY_N_MILLISECONDS(5000) {                                // FastLED based timer to update/display the sequence every 5 seconds.
    SetupRandomPalette();
  }
} //loop()


void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  uint8_t beatB = beatsin8(15, 10, 20);                       // Delta hue between LED's
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, 255, currentBlending);
    colorIndex += beatB;
  }
} //FillLEDsFromPaletteColors()


void SetupRandomPalette() {
  targetPalette = CRGBPalette16(CHSV(random8(), 255, 32), CHSV(random8(), random8(64)+192, 255), CHSV(random8(), 255, 32), CHSV(random8(), 255, 255)); 
} // SetupRandomPalette()