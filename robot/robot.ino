#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

int neoPin = 0;
int neoCount = 2; // number of pixels in strip
int speakerPin = 1;
int buttonPin = 2; // PB2 is the only interrupt pin (INT0) on the Trinket

Adafruit_NeoPixel strip = Adafruit_NeoPixel(neoCount, neoPin, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

const uint32_t OFF = strip.Color(0, 0, 0);
const uint32_t WHT = strip.Color(128, 128, 128);
const uint32_t GRN = strip.Color(0, 128, 50);
const uint32_t YEL = strip.Color(128, 128, 0);
const uint32_t RED = strip.Color(156, 0, 0);
const uint32_t BLU = strip.Color(0, 128, 255);

void setup() {
  pinMode(speakerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(0, bloop, FALLING);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  delay(30000);
  fade();
}

void bloop() {
  play(WHT, OFF, 100, 50);
  play(WHT, OFF, 150, 50);
  play(OFF, WHT, 220, 50);
  play(OFF, WHT, 44, 50);
  play(OFF, OFF, 130, 50);
  play(OFF, OFF, 330, 50);
  play(OFF, WHT, 44, 50);
  play(OFF, WHT, 330, 50);
  play(WHT, OFF, 44, 50);
  play(WHT, OFF, 180, 50);
  play(OFF, OFF, 330, 50);
  play(OFF, WHT, 150, 50);
  play(WHT, OFF, 100, 50);
  doubleFlash(BLU);
  doubleFlash(GRN);
}

void fade() {
  for (uint16_t i = 0; i < 128; i++) {
    uint32_t c = strip.Color(i, 0, 0);
    strip.setPixelColor(0, c);
    strip.setPixelColor(1, c);
    strip.show();
    delay(10);
  }
  for (uint16_t i = 127; i >= 0; i--) {
    uint32_t c = strip.Color(i, 0, 0);
    strip.setPixelColor(0, c);
    strip.setPixelColor(1, c);
    strip.show();
    delay(10);
  }
}

void doubleFlash(uint32_t c) {
  play(c, c, 44, 50);
  delay(100);
  play(c, c, 44, 50);
  delay(900);  
}

void play(uint32_t left, uint32_t right, unsigned int freq, unsigned int duration) {
  strip.setPixelColor(0, left);
  strip.setPixelColor(1, right);
  strip.show();
  if (freq == 0) {
    delay(duration);
  } else {
    playTone(freq, duration);    
  }
  strip.setPixelColor(0, OFF);
  strip.setPixelColor(1, OFF);
  strip.show();
}

void playTone(unsigned int freq, unsigned int duration) {
  unsigned long d = duration * 1000L;
  unsigned long l = 1000000L / freq;
  unsigned long elapsed = 0;
  while (elapsed < d) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(l / 2);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(l / 2);
    elapsed += l;
  }
}

