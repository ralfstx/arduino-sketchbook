#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6
#define COUNT 5 // number of pixels in strip

Adafruit_NeoPixel strip = Adafruit_NeoPixel(COUNT, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  fadeIn();
  delay(500);
//  colorWipe(strip.Color(255, 0, 0), 50); // Red
//  colorWipe(strip.Color(0, 255, 0), 50); // Green
//  colorWipe(strip.Color(0, 0, 255), 50); // Blue
//  colorWipe(strip.Color(255, 255, 255), 50); // White
  fadeOut();
  delay(500);
  flash();
  delay(100);
  flash();
  delay(500);
  flash();
  delay(100);
  flash();
  delay(500);
//  delay(1000);
//  rainbow(50);
//  delay(500);
//  rainbowCycle(50);
//  delay(500);
}

void flash() {
  uint32_t c = strip.Color(128, 0, 0);
  strip.setPixelColor(1, c);
  strip.setPixelColor(3, c);
  strip.show();
  delay(20);  
  c = strip.Color(0, 0, 0);
  strip.setPixelColor(1, c);
  strip.setPixelColor(3, c);
  strip.show();
}

void fadeIn() {
  for (uint16_t i = 0; i < 128; i++) {
    uint32_t c = strip.Color(i, 0, 0);
    for (uint16_t p = 0; p < strip.numPixels(); p++) {
      strip.setPixelColor(p, c);
    }
    strip.show();
    delay(20);
  }
  for (uint16_t i = 0; i < 128; i++) {
    uint32_t c = strip.Color(127, i, 0);
    for (uint16_t p = 0; p < strip.numPixels(); p++) {
      strip.setPixelColor(p, c);
    }
    strip.show();
    delay(20);
  }
  for (uint16_t i = 0; i < 128; i++) {
    uint32_t c = strip.Color(127, 127, i);
    for (uint16_t p = 0; p < strip.numPixels(); p++) {
      strip.setPixelColor(p, c);
    }
    strip.show();
    delay(20);
  }
}

void fadeOut() {
  for (uint16_t i = 0; i < 256; i++) {
    uint32_t c = strip.Color(255 - i, 255, 255);
    for (uint16_t p = 0; p < strip.numPixels(); p++) {
      strip.setPixelColor(p, c);
    }
    strip.show();
    delay(20);
  }
  for (uint16_t i = 0; i < 256; i++) {
    uint32_t c = strip.Color(0, 255 - i, 255);
    for (uint16_t p = 0; p < strip.numPixels(); p++) {
      strip.setPixelColor(p, c);
    }
    strip.show();
    delay(20);
  }
  for (uint16_t i = 0; i < 256; i++) {
    uint32_t c = strip.Color(0, 0, 255 - i);
    for (uint16_t p = 0; p < strip.numPixels(); p++) {
      strip.setPixelColor(p, c);
    }
    strip.show();
    delay(20);
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;
  for(j = 0; j < 256; j++) {
    for(i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j = 0; j < 256 * 3; j++) { // 3 cycles of all colors on wheel
    for(i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t wheel(byte hue) {
  hue = 255 - hue;
  if(hue < 85) {
    return strip.Color(255 - hue * 3, 0, hue * 3);
  }
  if(hue < 170) {
    hue -= 85;
    return strip.Color(0, hue * 3, 255 - hue * 3);
  }
  hue -= 170;
  return strip.Color(hue * 3, 255 - hue * 3, 0);
}

