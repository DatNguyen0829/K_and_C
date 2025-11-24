#include <Arduino.h>
#include "HX711.h"

const uint8_t SCK_PIN = 3;
const uint8_t DATAOUT_PIN = 2;

HX711 scale;

void setup() {
    Serial.begin(38400);
    Serial.println("Setting up the scale");

    //Scale initialized 
    scale.begin(DATAOUT_PIN, SCK_PIN);



}

void loop() {
  
}
