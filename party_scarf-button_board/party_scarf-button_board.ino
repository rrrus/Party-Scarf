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
bool button1Held = false;
long button1timer = 0;
bool button2Held = false;
long button2timer = 0;


Button button1 = Button(BUTTON_1, HIGH);
Button button2 = Button(BUTTON_2, HIGH);

void (*patterns[])() = {rainbow, bpm_rainbow, wipe, marquee, marquee_aa};
int brightness_values[] = {20, 50, 100, 200};
float speeds[] = {0.05, 0.2, 1.0};

int num_patterns = sizeof(patterns) / sizeof(patterns[0]);
int num_bright = sizeof(brightness_values) / sizeof(brightness_values[0]);
int num_spd = sizeof(speeds) / sizeof(speeds[0]);

int current_pattern = 0;

int count = 0;
long msecsFirst = 0;
long msecsPrevious = 0;
float bpmAvg = 120.0;

void setup() {
  FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(brightness_values[brightness]);
  button1.setDebounceDelay(50);
  button2.setDebounceDelay(50);
  button1.setHoldDelay(300);
  Serial.begin(9600);
  Serial.print("Total number of patterns: ");
  Serial.println(num_patterns);

}

void loop()
{
  button1.listen();
  button2.listen();

  if (button1.onRelease()) {
    if (!button1Held) {
      changePattern();
      FastLED.setBrightness(brightness_values[brightness % num_bright]);
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

    if (button2.onRelease()) {
      if(!button2Held) {
        tapTempo();
        } else {
          button2Held = false;
        }
      }

      if (button2.isHold()) {
        button2Held = true;
        if (millis() - button2timer > 300) {
          button2timer = millis();
          changeSpeed();
        }
      }


      patterns[current_pattern % num_patterns]();

  FastLED.show(); // display this frame
}

void rainbow() {
  fill_rainbow(leds, NUM_LEDS, wheel_f, 2);
  wheel_f = wheel_f + speeds[spd % num_spd];
}

void bpm_rainbow() {
  int b = brightness_values[brightness % num_bright];
  FastLED.setBrightness(beatsin8(bpmAvg, 0, b));
  rainbow();
}

void wipe() {
  FastLED.clearData();
  fill_rainbow(leds, beatsin8(bpmAvg, 0, NUM_LEDS), wheel_f, 2);
  wheel_f = wheel_f + speeds[spd % num_spd];
}

#define marquee_skip 4
float start_led = 0.0;
void marquee() {
  FastLED.clearData();
  int s = start_led;
  for (int i = (s % marquee_skip); i < NUM_LEDS; i = i + marquee_skip) {
    leds[i] = CRGB::WhiteSmoke;
  }
  start_led = beatsin8(bpmAvg, 0, 100) / (100 / marquee_skip);
  //start_led = start_led + (speeds[spd % num_spd] / 3);
}

void marquee_aa() {
  FastLED.clearData();
  int s = start_led;
  int bright = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    // s = 2
    // start_led = 2.2
    // i = 1
    // bright = 0
    // s % marquee_skip
    float v = fmod(i, start_led) - (marquee_skip - 1);
    Serial.print("StartLED: ");
    Serial.print(start_led);
    Serial.print(", i: ");
    Serial.print(i);
    Serial.print(", v: ");
    Serial.println(v);
    if (v > 0) {
      bright = 255 * v;
    } else {
      bright = 255;
    }
    leds[i] = CHSV(0, 0, bright);
  }
  start_led = beatsin8(bpmAvg, 0, 100) / (100 / marquee_skip);
  //start_led = start_led + (speeds[spd % num_spd] / 3);
}

void changeBrightness() {
  brightness++;
  FastLED.setBrightness(brightness_values[brightness % num_bright]);
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
    bpmAvg = 60000.0 * count / (msecs - msecsFirst);
    count++;
  }
  msecsPrevious = msecs;
}

