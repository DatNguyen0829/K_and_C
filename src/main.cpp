#include <Arduino.h>
#include "HX711.h"

//---Pin configs---

const uint8_t SCK_PIN = 3;
const uint8_t DATAOUT_PIN = 2;

//---Variables---
HX711 scale;
long calibration_factor = 1;

//---Functions---
long start_calibration(); //Calibrate the scale based on the a known weight to get the calibration factor

void setup() {
    Serial.begin(38400);
    Serial.println("Setting up the scale");

    //Scale initialized 
    scale.begin(DATAOUT_PIN, SCK_PIN);
    scale.set_scale(calibration_factor); //Set scale based on factor
    scale.tare(); //0 the scale

}

void loop() {
  Serial.print("one reading:\t");
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| average:\t");
  Serial.println(scale.get_units(10), 5);

  delay(5000);
}

long start_calibration(){
  if (scale.is_ready()) {
    scale.set_scale();    
    Serial.println("Tare... remove any weights from the scale.");
    delay(5000);
    scale.tare();
    Serial.println("Tare done...");
    Serial.print("Place a known weight on the scale...");
    delay(5000);
    long reading = scale.get_units(10);
    Serial.print("Result: ");
    Serial.println(reading);
    calibration_factor = reading/100;
  } 
  else {
    Serial.println("HX711 not found.");
  }
  delay(1000);
}