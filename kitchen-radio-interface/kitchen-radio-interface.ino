/*

  Example for controlling a white 1.3" SPI 128X64 OLED LCD display

  Using Universal 8bit Graphics Library, http://code.google.com/p/u8glib/

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  -----------------------------------------------------------------------------------------

  Wiring
  ------

  The display has 6 pins, connected to the Arduino Pro Mini like this:

  LCD       Pro Mini

  VCC  -->  3.3V
  GND  -->  GND
  CLK  -->  13 (SCK)
  MOSI -->  11 (MOSI)
  CS   -->  9
  D/C  -->  8

  Controlling
  -----------

  The display can be controlled via terminal with the following command set:

  HEAD <text> Sets the text for the headline.
  LIN<n> <text> Sets the text for line n. n = [1..3])
  PROG <n> - Sets the progress bar. n = [0..100]
  STAT <n> - Sets the status n = 0: off, 1: play, 2: pause
  REFR - Refreshes the display.
 */

#define CS_PIN 9
#define A0_PIN 8

#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(CS_PIN, A0_PIN);

const uint8_t PLAY_BITMAP[] U8G_PROGMEM = {
  0x80, // 10000000
  0xe0, // 11100000
  0xf8, // 11111000
  0xfe, // 11111110
  0xff, // 11111111
  0xfe, // 11111110
  0xf8, // 11111000
  0xe0, // 11100000
  0x80  // 10000000
};

const uint8_t PAUSE_BITMAP[] U8G_PROGMEM = {
  0xe7, // 11100111
  0xe7, // 11100111
  0xe7, // 11100111
  0xe7, // 11100111
  0xe7, // 11100111
  0xe7, // 11100111
  0xe7, // 11100111
  0xe7, // 11100111
  0xe7  // 11100111
};

// Serial config

const int MAX_CMD_LEN = 80;

char serialBuffer[MAX_CMD_LEN + 1];
int serialBufferFill = 0;

// Display data

int status = 0;
int progress = 0;
char head[21];
char line1[21];
char line2[21];
char line3[21];

void drawStatus() {
  int x = 128 - 16, y = 0;
  if (status == 1) {
    u8g.drawBitmapP(x + 4, y + 4, 1, 9, PLAY_BITMAP);
  } else if (status == 2) {
    u8g.drawBitmapP(x + 4, y + 4, 1, 9, PAUSE_BITMAP);
  }
}

void drawProgress() {
  int x = 0, y = 16;
  u8g.drawBox(x, y, progress * 128 / 100, 2);
}

void drawHead() {
  int x = 0, y = 14;
  u8g.setFont(u8g_font_7x14);
  u8g.drawStr(x, y, head);
}

void drawLine(char *line, int n) {
  int x = 0, y = 19;
  u8g.setFont(u8g_font_7x13);
  u8g.drawStr(x, y + n * 14, line);
}

void draw() {
  drawStatus();
  drawProgress();
  drawHead();
  drawLine(line1, 1);
  drawLine(line2, 2);
  drawLine(line3, 3);
}

void refresh() {
  u8g.firstPage();
  do {
    draw();
  } while (u8g.nextPage());
}

void setInt(int *dst, char *str, int min, int max) {
  char* end = str;
  long int result = strtol(str, &end, 10);
  if (!*end && result >= min && result <= max) {
    *dst = result;
    Serial.println("OK");
  } else {
    Serial.println("ERROR: Illegal value");
  }
}

void setLine(char *dst, char *str, int len) {
  strncpy(dst, str, len);
  dst[len] = 0;
  Serial.println("OK");
}

void processSerialCmd(char *line) {
  Serial.println(line);
  char cmd[5];
  char *arg = "";
  cmd[4] = 0;
  strncpy(cmd, line, 4);
  if (strlen(line) > 4) {
    arg = line + 4;
  }
  while (isspace(*arg)) ++arg;
  if (strcmp("HEAD", cmd) == 0) {
    setLine(head, arg, 20);
  } else if (strcmp("LIN1", cmd) == 0) {
    setLine(line1, arg, 20);
  } else if (strcmp("LIN2", cmd) == 0) {
    setLine(line2, arg, 20);
  } else if (strcmp("LIN3", cmd) == 0) {
    setLine(line3, arg, 20);
  } else if (strcmp("STAT", cmd) == 0) {
    setInt(&status, arg, 0, 2);
  } else if (strcmp("PROG", cmd) == 0) {
    setInt(&progress, arg, 0, 100);
  } else if (strcmp("REFR", cmd) == 0) {
    refresh();
  } else {
    Serial.println("ERROR: Invalid command");
  }
}

void readSerial() {
  while (Serial.available() > 0) {
    char ch = Serial.read();
    if (ch == '\n') {
      processSerialCmd(serialBuffer);
      serialBufferFill = 0;
    } else if (serialBufferFill < MAX_CMD_LEN) {
      serialBuffer[serialBufferFill++] = ch;
    }
    serialBuffer[serialBufferFill] = 0;
  }
}

//void readButtons() {
//  int actual = digitalRead(buttonPin);
//  if (actual != buttonState) {
//    buttonState = actual;
//    if (actual && (millis() - lastDebounceTime) > debounceDelay) {
//      Serial.println("button");
//    }
//    lastDebounceTime = millis();
//  }
//}

void setup(void) {
  u8g.setColorIndex(1);
  setLine(line1, "Loading", 10);
  refresh();
  Serial.begin(9600);
}

void loop(void) {
  readSerial();
  delay(50);
}

