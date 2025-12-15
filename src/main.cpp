#include <Arduino.h>
const float MAX_VOLTAGE = 5.0;
const float MAX_TRAVEL_IN = 10.0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int raw = analogRead(A0);            // 0–1023
  float voltage = raw * (5.0 / 1023.0);
  float displacement_in = (voltage / MAX_VOLTAGE) * MAX_TRAVEL_IN;

  Serial.print(displacement_in, 3);
  Serial.println(" in");

  delay(100);
}
