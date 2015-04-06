/*

  Reading multiple buttons, reporting button presses and longpresses via serial.

  Wiring
  ------

  GND -- R10k -- BTN --+--> PIN (using internal pull-up resistor)
                       |
  GND -- C1u  --------/

  BTN1  -->   9
  BTN2  -->  10
  BTN3  -->  11
  BTN4  -->  12

*/

const int BUTTON_COUNT = 4;
const int BUTTON_PINS[] = {9, 10, 11, 12};
const int DEBOUNCE_DELAY = 50;
const int LONGPRESS_DELAY = 800;

const int PRESSED = LOW;
const int RELEASED = HIGH;
const int LONGPRESS = 2;

int buttonStates[BUTTON_COUNT] = { RELEASED };
int lastButtonStates[BUTTON_COUNT] = { RELEASED };
long lastButtonDebounce[BUTTON_COUNT] = { 0 };

void readButtons() {
  for (int i = 0; i < BUTTON_COUNT; i++) {
    int actual = digitalRead(BUTTON_PINS[i]);
    if (buttonStates[i] != actual && buttonStates[i] != LONGPRESS) {
      buttonStates[i] = actual;
      if (actual == RELEASED && millis() - lastButtonDebounce[i] > DEBOUNCE_DELAY) {
        Serial.print("BUTTON ");
        Serial.println(i + 1);
      }
      lastButtonDebounce[i] = millis();
    } else if (buttonStates[i] == LONGPRESS && actual == RELEASED) {
      buttonStates[i] = actual;
    } else if (buttonStates[i] == PRESSED && actual == PRESSED) {
      if (millis() - lastButtonDebounce[i] > LONGPRESS_DELAY) {
        buttonStates[i] = LONGPRESS;
        Serial.print("LONGPRESS ");
        Serial.println(i + 1);
      }
    }
  }
}

void setup(void) {
  for (int i = 0; i < BUTTON_COUNT; i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }
  Serial.begin(9600);
}

void loop(void) {
  readButtons();
  delay(20);
}

