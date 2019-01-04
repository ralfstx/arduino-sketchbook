/*
  Controlling the white "HelTec" 1.3" SPI 128X64 OLED LCD display (3.3V)
  with an Arduino Pro Mini (8MHz, 3.3V)

  Using Universal 8bit Graphics Library, http://code.google.com/p/u8glib/

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
  CHR<n> - Shows the nth block of characters. n = [0..3]
  REFR - Refreshes the display.

  Character Set
  -------------
  The display supports all Latin-1 characters.
  The terminal sender must encode strings in Latin-1.
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
    Serial1.println("OK");
  } else {
    Serial1.println("ERROR: Illegal value");
  }
}

void setLine(char *dst, char *str, int len) {
  strncpy(dst, str, len);
  dst[len] = 0;
  Serial1.println("OK");
}

void showChars(int start) {
  char myChar1[17];
  char myChar2[17];
  char myChar3[17];
  char myChar4[17];
  for (int i = 0; i < 16; i++) {
    myChar1[i] = start + i;
    myChar2[i] = start + i + 16;
    myChar3[i] = start + i + 32;
    myChar4[i] = start + i + 48;
  }
  setLine(head, myChar1, 16);
  setLine(line1, myChar2, 16);
  setLine(line2, myChar3, 16);
  setLine(line3, myChar4, 16);
}

void processSerialCmd(char *line) {
  Serial1.println(line);
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
  } else if (strcmp("CHR0", cmd) == 0) {
    showChars(0);
  } else if (strcmp("CHR1", cmd) == 0) {
    showChars(64);
  } else if (strcmp("CHR2", cmd) == 0) {
    showChars(128);
  } else if (strcmp("CHR3", cmd) == 0) {
    showChars(192);
  } else if (strcmp("REFR", cmd) == 0) {
    refresh();
  } else {
    Serial1.println("ERROR: Invalid command");
  }
}

void readSerial() {
  while (Serial1.available() > 0) {
    char ch = Serial1.read();
    if (ch == '\n') {
      processSerialCmd(serialBuffer);
      serialBufferFill = 0;
    } else if (serialBufferFill < MAX_CMD_LEN) {
      serialBuffer[serialBufferFill++] = ch;
    }
    serialBuffer[serialBufferFill] = 0;
  }
}

void setup(void) {
  u8g.setColorIndex(1);
  setLine(line1, "Loading...", 16);
  refresh();
  Serial1.begin(9600);
  Serial1.println("HI");
}

void loop(void) {
  readSerial();
  delay(50);
}
