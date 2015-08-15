#include <FastLED.h>

#define LED_PIN     13
#define CLOCK_PIN   12
#define BUTTON_1    4
#define BUTTON_2    3
#define COLOR_ORDER GBR
#define CHIPSET     APA102
#define NUM_LEDS    48

#define BRIGHTNESS  100
#define FRAMES_PER_SECOND 20

CRGB leds[NUM_LEDS];
uint8_t wheelpos = 0;

int brightness = 1;
int brightness_values[] = {20, 100, 150};
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers
void setup() {
  //delay(3000); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(brightness_values[brightness]);
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  Serial.begin(9600);

}

void loop()
{
  buttonState = digitalRead(BUTTON_1);
  if (buttonState == HIGH) {
    changeBrightness();
  }
  rainbow(); // run simulation frame

  FastLED.show(); // display this frame
  //FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void rainbow() {
  fill_rainbow(leds, NUM_LEDS, wheelpos, 2);
  wheelpos++;
}

void changeBrightness() {
  brightness++;
  FastLED.setBrightness(brightness_values[brightness % 3]);
}

