#include <Arduino.h>

void setup() {
  init();

  Serial.begin(9600);
  Serial3.begin(9600);
}

int main() {
  setup();

  Serial.println("I am Arduino B");

  while (true) {
    // if there is a byte to read from Arduino A,
    // read it and send it to the serial monitor
    if (Serial3.available() > 0) {
      uint8_t byte_read = Serial3.read();
      Serial.write(byte_read);
    }
  }

  // should not get here because the loop above is infinite
  return 0;
}
