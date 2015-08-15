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
float wheel_f = 0.0;

int brightness = 1;
int spd = 0;
int brightness_values[] = {20, 50, 100, 200};
float speeds[] = {0.2, 1.0};
bool button1Held = false;
long button1timer = 0;

Button button1 = Button(BUTTON_1, HIGH);
Button button2 = Button(BUTTON_2, HIGH);

void (*patterns[])() = {rainbow, wipe};
int current_pattern = 0;

int count = 0;
long msecsFirst = 0;
long msecsPrevious = 0;
float bpmAvg = 120;

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
      changePattern();
    } else {
        button1Held = false;
    }
  }

  if (button1.isHold()) {

    button1Held = true;
    if (millis() - button1timer > 300) {
        button1timer = millis();
        changeBrightness();
    }
  }

  if (button2.onPress()) {
    tapTempo();
  }


  patterns[current_pattern % 2]();

  FastLED.show(); // display this frame
  //FastLED.delay(1000 / FRAMES_PER_SECOND);

}

void rainbow() {
  fill_rainbow(leds, NUM_LEDS, wheel_f, 2);
  wheel_f = wheel_f + speeds[spd % 2];
}

void wipe() {
  FastLED.clearData();
  fill_rainbow(leds, beatsin8(bpmAvg, 0, NUM_LEDS), wheel_f, 2);
  wheel_f = wheel_f + speeds[spd % 2];
}

void changeBrightness() {
  brightness++;
  FastLED.setBrightness(brightness_values[brightness % 4]);
}

void changeSpeed() {
  spd++;
}

void changePattern() {
  current_pattern++;
}

void tapTempo() {
  long msecs = millis();
  if ((msecs - msecsPrevious) > 1000 * 2)
  {
    count = 0;
  }

  if (count == 0)
  {
    msecsFirst = msecs;
    count = 1;
  }
  else
  {
    bpmAvg = 60000 * count / (msecs - msecsFirst);
    count++;
  }
  msecsPrevious = msecs;
}

