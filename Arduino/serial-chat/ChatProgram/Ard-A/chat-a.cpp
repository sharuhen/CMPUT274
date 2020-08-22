#include <Arduino.h>

void setup() {
  init();

  Serial.begin(9600);
  Serial3.begin(9600);
}

int main() {
  setup();

  Serial.println("I am Arduino A");

  while (true) {
    // if there is a byte to read, read it and send to Arduino B
    if (Serial.available() > 0) {
      uint8_t byte_read = Serial.read();
      Serial3.write(byte_read);
    }
  }

  // should not get here because the loop above is infinite
  return 0;
}
