

#define qsubd(x, b)  ((x>b)?wavebright:0)                     // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0

// Initialize changeable global variables. Play around with these!!!
uint8_t wavebright = 128;                                     // You can change the brightness of the waves/bars rolling across the screen.
uint8_t thishue = 0;                                          // You can change the starting hue value for the first wave.
uint8_t thisrot = 1;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
uint8_t allsat = 255;                                         // I like 'em fully saturated with colour.
int8_t thisspeed = 8;                                         // You can change the speed of the wave, and use negative values.
uint8_t allfreq = 32;                                         // You can change the frequency, thus distance between bars.
int thisphase = 0;                                            // Phase change value gets calculated.
uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
int onesine_thisdelay = 30;                                           // You can change the delay. Also you can change the allspeed variable above. 
uint8_t bgclr = 0;                                            // A rotating background colour.
uint8_t bgbri = 16;                                           // Brightness of background colour


void onesine () {
  EVERY_N_MILLISECONDS(onesine_thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    one_sine();
  }
} // loop()


void one_sine() {                                                             // This is the heart of this program. Sure is short.
  thisphase += thisspeed;                                                     // You can change direction and speed individually.
  thishue = thishue + thisrot;                                                // Hue rotation is fun for thiswave.
  for (int k=0; k<NUM_LEDS-1; k++) {                                          // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int thisbright = qsubd(cubicwave8((k*allfreq)+thisphase), thiscutoff);    // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] = CHSV(bgclr, 255, bgbri);                                        // First set a background colour, but fully saturated.
    leds[k] += CHSV(thishue, allsat, thisbright);                             // Then assign a hue to any that are bright enough.
  }
  bgclr++;                                                                    // You can change the background colour or remove this and leave it fixed.
} // one_sine()