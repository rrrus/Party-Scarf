#include <FastLED.h>
#include <Button.h>

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
bool button1Held = false;
long button1timer = 0;
Button button1 = Button(BUTTON_1, HIGH);
Button button2 = Button(BUTTON_2, HIGH);

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(brightness_values[brightness]);
  button1.setDebounceDelay(50);
  button2.setDebounceDelay(50);
  button1.setHoldDelay(300);
  Serial.begin(9600);

}

void loop()
{
  button1.listen();
  button2.listen();

  if (button1.onRelease()) {
    if (!button1Held) {
      Serial.println("Button 1 pressed");
    } else {
        button1Held = false;
    }
  }

  if (button1.isHold()) {

    button1Held = true;
    if (millis() - button1timer > 300) {
        button1timer = millis();
        Serial.println("Button 1 held");
    }
  }


  rainbow(); // run simulation frame

  FastLED.show(); // display this frame
  //FastLED.delay(1000 / FRAMES_PER_SECOND);

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

