#include <FastLED.h>

#define LED_PIN     13
#define CLOCK_PIN   12
#define BUTTON_1    2
#define BUTTON_2    3
#define COLOR_ORDER GBR
#define CHIPSET     APA102
#define NUM_LEDS    48

#define BRIGHTNESS  100
#define FRAMES_PER_SECOND 20

CRGB leds[NUM_LEDS];
uint8_t wheelpos = 0;
float wheel_f = 0;

int brightness = 1;
int spd = 0;
int brightness_values[] = {20, 50, 100, 200};
int speeds[] = {0.05, 0.1, 0.5, 1.0};
int buttonState1;             // the current reading from the input pin
int buttonState2;             // the current reading from the input pin
int lastButtonState1 = LOW;   // the previous reading from the input pin
int lastButtonState2 = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime1 = 0;  // the last time the output pin was toggled
long lastDebounceTime2 = 0;  // the last time the output pin was toggled

long holdTime1 = 0;
long holdTime2 = 0;

long debounceDelay = 20;    // the debounce time; increase if the output flickers
void setup() {
  //delay(3000); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(brightness_values[brightness]);
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  //Serial.begin(9600);

}

void loop()
{
  // read the state of the switch into a local variable:
  int reading1 = digitalRead(BUTTON_1);
  int reading2 = digitalRead(BUTTON_2);
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading1 != lastButtonState1) {
    // reset the debouncing timer
    lastDebounceTime1 = millis();
  }

  if (reading2 != lastButtonState2) {
    // reset the debouncing timer
    lastDebounceTime2 = millis();
  }

  if ((millis() - lastDebounceTime1) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading1 != buttonState1) {
      buttonState1 = reading1;

      // only toggle the LED if the new button state is HIGH
      if (buttonState1 == HIGH) {
        changeBrightness();
      }
    }
  }

  if ((millis() - lastDebounceTime2) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading2 != buttonState2) {
      buttonState2 = reading2;

      // only toggle the LED if the new button state is HIGH
      if (buttonState2 == HIGH) {
        changeSpeed();
      }
    }
  }

  rainbow(); // run simulation frame

  FastLED.show(); // display this frame
  //FastLED.delay(1000 / FRAMES_PER_SECOND);
  lastButtonState1 = reading1;
  lastButtonState2 = reading2;
}

void rainbow() {
  fill_rainbow(leds, NUM_LEDS, wheel_f, 2);
  wheel_f = wheel_f + speeds[spd % 4];
}

void changeBrightness() {
  brightness++;
  FastLED.setBrightness(brightness_values[brightness % 4]);
}

void changeSpeed() {
  spd++;

}

