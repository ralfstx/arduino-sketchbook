#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

int led = 1;
int pin = 0;
int count = 2; // number of pixels in strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(count, pin, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  pinMode(led, OUTPUT);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  doubleFlash(strip.Color(255, 0, 255));
  doubleFlash(strip.Color(0, 0, 255));
}

void doubleFlash(uint32_t c) {
  digitalWrite(led, HIGH); 
  flash(c);
  delay(100);
  flash(c);
  digitalWrite(led, LOW);
  delay(900);  
}

void flash(uint32_t c) {
  strip.setPixelColor(0, c);
  strip.setPixelColor(1, c);
  strip.show();
  delay(50);
  c = strip.Color(0, 0, 0);
  strip.setPixelColor(0, c);
  strip.setPixelColor(1, c);
  strip.show();
}

