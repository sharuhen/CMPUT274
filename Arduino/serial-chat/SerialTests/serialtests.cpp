#include <Arduino.h>

void setup() {
  // needed for all Arduino programs
  init();

  Serial.begin(9600);
}

int main() {
  setup();

  // TESTING: counting bytes sent
  //int bytesSent = Serial.println("Testing");
  //int bytesSent = Serial.println(-47);
  int bytesSent = Serial.write(122);
  Serial.println();
  //Serial.println();
  Serial.print("The first message sent ");
  Serial.print(bytesSent);
  Serial.println(" bytes.");

  // TESTING: sending only \n or \r

  Serial.println("Hello\nWorld");
  Serial.println("A polite greeting is\rhi");
  Serial.println("The proper way\r\nto do a newline yourself");

  // TESTING: Serial.readBytes()
  char myArray[20];

  Serial.println("Waiting for 5 characters");
  while (Serial.available() < 5) {
    delay(10);
  }

  int numBytes = Serial.readBytes(myArray, 5);
  Serial.print("We read: ");
  // print each character we read
  for (int i = 0; i < numBytes; ++i) {
    Serial.print(myArray[i]);
  }
  Serial.println();

  // TESTING: Serial.read()

  Serial.println("Now waiting for a single byte");
  while (Serial.available() == 0) {
    delay(10);
  }
  // reads a single byte
  char myByte = Serial.read();
  Serial.print("The character read was: ");
  Serial.println(myByte);

  // try removing this "flush" command to see what happens
  Serial.flush();
  return 0;
}
