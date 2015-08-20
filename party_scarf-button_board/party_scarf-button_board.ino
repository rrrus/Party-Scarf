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

#define BRIGHT_HOLD_DELAY 1000

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

void (*patterns[])() = {onesine, juggle, ease_me, colorpal_beat, dot_beat, rainbow_beat, rainbow, bpm_rainbow, wipe, marquee};
int brightness_values[] = {20, 50, 100, 200};
float speeds[] = {0.05, 0.2, 1.0};

int num_patterns = sizeof(patterns) / sizeof(patterns[0]);
int num_bright = sizeof(brightness_values) / sizeof(brightness_values[0]);
int num_spd = sizeof(speeds) / sizeof(speeds[0]);

int current_pattern = 0;

int tap_count = 0;
long msecsFirst = 0;
long msecsPrevious = 0;
float bpmAvg = 120.0;

void setup() {
  //FastLED.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  LEDS.addLeds<CHIPSET, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS); 
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
      if (millis() - button1timer > BRIGHT_HOLD_DELAY) {
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
  int s = start_led;
  for (int i = (s % marquee_skip); i < NUM_LEDS; i = i + marquee_skip) {
    leds[i] = CRGB::WhiteSmoke;
  }
  start_led = beatsin8(bpmAvg, 0, 100) / (100 / marquee_skip);
  fadeToBlackBy(leds, NUM_LEDS, 16);                     // 8 bit, 1 = slow, 255 = fast
  //start_led = start_led + (speeds[spd % num_spd] / 3);
}

void rainbow_beat() {
  uint8_t beatA = beatsin8(5, 0, 255);                        // Starting hue
  uint8_t beatB = beatsin8(15, 4, 20);                        // Delta hue between LED's
  fill_rainbow(leds, NUM_LEDS, beatA, beatB);                 // Use FastLED's fill_rainbow routine.
} // rainbow_beat()

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
    tap_count = 0;
  }

  if (tap_count == 0)
  {
    msecsFirst = msecs;
    tap_count = 1;
  }
  else
  {
    bpmAvg = 60000.0 * tap_count / (msecs - msecsFirst);
    tap_count++;
  }
  msecsPrevious = msecs;
}

