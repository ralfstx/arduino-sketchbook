/**

  Example for controlling an Adafruit 1.27" OLED LCD display [1].
  This display has 128x96 pixel and uses the SSD1351 driver chip.

  Uses the Adafruit SSD1351 library [2] and the GFX Graphics core library [3].
  Requires an adjustment in the Adafruit_SSD1351 library:
  in Adafruit_SSD1351.h, change SSD1351HEIGHT to 96.

  LCD Pins
  --------

  Short  Long  Meaning
  front  back

  SI     MOSI  SPI MOSI Line ("Master Out, Slave In")
  CL     CLK   SPI Clock Line ("Serial Clock")
  DC     DC    Data/Command
  R      RST   Reset
  OC     OCS   OLED CS ("Chip Select" for the LCD)
  SC     SCS   SD CS ("Chip Select" for the SD card)
  SO     MISO  SPI MISO ("Master In, Slave Out")
  CD     CD    ?
  3V     3vo   3V output
  +      Vin
  G      Gnd


  Wiring
  ------

  LCD    Arduino Micro

  SI     MOSI
  CL     SCK
  DC     4     (can use any port)
  R      6     (can use any port)
  OC     5     (can use any port)
  +      5V
  G      GND

  [1] https://www.adafruit.com/products/1673
  [2] https://github.com/adafruit/Adafruit-SSD1351-library
  [3] https://github.com/adafruit/Adafruit-GFX-Library

  See also:
  https://learn.adafruit.com/ssd1306-oled-displays-with-raspberry-pi-and-beaglebone-black/wiring
  https://learn.adafruit.com/adafruit-gfx-graphics-library

 */

#define DC_PIN    4
#define CS_PIN    5
#define RST_PIN   6

#define	BLACK     0x0000
#define WHITE     0xFFFF
#define	RED       0xF800
#define	GREEN     0x07E0
#define	BLUE      0x001F
#define YELLOW    0xFFE0  

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

#define PROG_BAR_HEIGHT 3

// use the hardware SPI pins
Adafruit_SSD1351 tft = Adafruit_SSD1351(CS_PIN, DC_PIN, RST_PIN);

int width = tft.width();
int height = tft.height();
int progress = 0;

void setup(void) {
  tft.begin();
  tft.fillScreen(BLACK);
  drawText();
  drawShape();
  drawColors();
}

void loop() {
  progress = (progress + 2) % 101;
  drawProgress(progress);
  delay(200);
}

void drawText() {
  tft.setTextColor(RED, BLACK);
  tft.setTextSize(1);
  tft.setTextWrap(false);
  tft.setCursor(2, 2);
  tft.println("It works!");
}

void drawShape() {
  int cx = width / 2;
  int cy = height / 2;
  tft.fillCircle(cx, cy, 15, BLUE);
  tft.drawFastHLine(cx - 20, cy - 1, 41, YELLOW);
  tft.drawFastHLine(cx - 20, cy + 1, 41, YELLOW);
}

void drawColors() {
  int y = height - 20;
  int x, c;
  for (int i = 0; i < 10; i++) {
    x = 7 * i;
    c = i * 3;
    tft.fillRect(x, y, 5, 5, c);
  }
  y += 7;
  for (int i = 0; i < 10; i++) {
    x = 7 * i;
    c = (i * 6) << 5;
    tft.fillRect(x, y, 5, 5, c);
  }
}

//int col(r, g, b) {
//  return (r << 11) + (g << 5) + b;
//}

int currPWidth = 0;

void drawProgress(int progress) {
  int newPWidth = progress * width / 100;
  if (newPWidth < currPWidth) {
    tft.fillRect(newPWidth, height - PROG_BAR_HEIGHT, currPWidth - newPWidth, PROG_BAR_HEIGHT, BLACK);
  } else if (newPWidth > currPWidth) {
    tft.fillRect(currPWidth, height - PROG_BAR_HEIGHT, newPWidth - currPWidth, PROG_BAR_HEIGHT, GREEN);
  }
  currPWidth = newPWidth;
}

