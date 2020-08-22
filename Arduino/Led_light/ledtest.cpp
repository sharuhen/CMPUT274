#include <Arduino.h>

// not a variable declaration
// rather, the compiler just uses 13 in the
// code whenever it sees MY_LED_PIN
#define MY_LED_PIN 13

void setup() {
  // needed for all Arduino programs
  init();

  Serial.begin(9600);

  pinMode(MY_LED_PIN, OUTPUT);
}

int main() {
  setup();

  int delay_on = 250;
  int delay_off = 750;

  while (true) {
    Serial.println("LED ON!");
    digitalWrite(MY_LED_PIN, HIGH);
    delay(delay_on);

    Serial.println("LED OFF!");
    digitalWrite(MY_LED_PIN, LOW);
    delay(delay_off);
  }

  Serial.flush();
  return 0;
}
