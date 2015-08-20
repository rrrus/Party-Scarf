// Routine specific variables
uint8_t    numdots =   4;                                     // Number of dots in use.
uint8_t   faderate =   2;                                     // How long should the trails be. Very low value = longer trails.
uint8_t     hueinc =  80;                                     // Incremental change in hue between each dot.
uint8_t    juggle_thishue =   0;                                     // Starting hue.
uint8_t     curhue =   0;                                     // The current hue
uint8_t    thissat = 255;                                     // Saturation of the colour.
uint8_t thisbright = 255;                                     // How bright should the LED/display be.
uint8_t   basebeat =   5;                                     // Higher = faster movement.

void juggle_patt() {
  //juggle_ChangeMe();
  juggle();  
} // loop()


void juggle() {                                               // Several colored dots, weaving in and out of sync with each other
  curhue = juggle_thishue;                                           // Reset the hue values.
  fadeToBlackBy(leds, NUM_LEDS, faderate);
  for( int i = 0; i < numdots; i++) {
    leds[beatsin16(basebeat+i+numdots,0,NUM_LEDS)] += CHSV(curhue, thissat, thisbright);   //beat16 is a FastLED 3.1 function
    curhue += 255/numdots;
  }
} // juggle()
