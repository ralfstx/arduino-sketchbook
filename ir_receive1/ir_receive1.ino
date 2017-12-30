/*
 * When a IR code is received, flash the built-in LED and print the code to serial.
 */

#include <IRremote.h>

const int RECV_PIN = 11;
const int LED_PIN = 13;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    digitalWrite(LED_PIN, HIGH);
    delay(50);
    digitalWrite(LED_PIN, LOW);
    Serial.println(results.value, HEX);
    Serial.println(results.value, BIN);
    Serial.println();
    irrecv.resume(); // Receive the next value
  }
}

