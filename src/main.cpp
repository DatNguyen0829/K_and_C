#include <Arduino.h>

uint8_t EXT_PIN = 5; //PWM pin to extend the actuator
uint8_t RET_PIN = 6; //PWm pin to retract the actuator
uint8_t PIN_R_ENABLE  = 7;   // enable R side (on the motor driver)
uint8_t PIN_L_ENABLE  = 8;   // enable L side (on the motor driver)

const uint8_t PWM_SPEED = 100; //The speed at which the actuator are moving 
const uint8_t DELAY_TIME = 8000;

void setup() {

  pinMode(EXT_PIN, OUTPUT);
  pinMode(RET_PIN, OUTPUT);

  pinMode(PIN_R_ENABLE,OUTPUT);
  pinMode(PIN_L_ENABLE,OUTPUT);

}

void loop() {
  delay(2000);
  Extend();
  delay(DELAY_TIME);
  Stop();
  delay(DELAY_TIME);
  Retract();
  delay(DELAY_TIME);
  
}

void Extend() {
  //Enable movement
  digitalWrite(PIN_R_ENABLE, HIGH);
  digitalWrite(PIN_L_ENABLE, HIGH);

  //Extend actuator
  analogWrite(EXT_PIN, PWM_SPEED);
  analogWrite(RET_PIN, 0);
}

void Retract() {
  //Enable movement
  digitalWrite(PIN_R_ENABLE, HIGH);
  digitalWrite(PIN_L_ENABLE, HIGH);

  //Retract actuator
  analogWrite(EXT_PIN, 0);
  analogWrite(RET_PIN, PWM_SPEED);
}

void Stop() {
  //Disable movement
  digitalWrite(PIN_R_ENABLE, LOW);
  digitalWrite(PIN_L_ENABLE, LOW);

  //Stop actuator
  analogWrite(EXT_PIN, 0);
  analogWrite(RET_PIN, 0);
}