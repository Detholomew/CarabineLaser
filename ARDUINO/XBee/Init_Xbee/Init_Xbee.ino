#include "Arduino.h"

void setup() {
  Serial.begin(9600);
  Serial.println("Test Receiver");
}

void loop() {

  if (Serial.available())
  {
    if(char ch = Serial.read())
        Serial.print(ch);
  }

}

