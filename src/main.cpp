#include <Arduino.h>

uint8_t EXT_PIN = 5; //PWM pin to extend the actuator
uint8_t RET_PIN = 6; //PWm pin to retract the actuator
uint8_t PIN_R_ENABLE  = 7;   // enable R side (on the motor driver)
uint8_t PIN_L_ENABLE  = 8;   // enable L side (on the motor driver)
uint8_t ENCODER_A = 2; //connected to channel A on encoder (for interrupt)

bool direction = 1;  //0: Retract. 1: Extend
volatile unsigned long pulseCounter = 0; //Counter for the pulse from encoder's channel A 
unsigned long lastPulseTime = 0; // Time stamp of last pulse
unsigned int trigDelay = 500;            // Delay bewteen pulse in microseconds
long position = 0; //Position from pulse counted 
long previousePosition = 0; //Tracks previous position to see if the actuator is moving  
const float conNum = 0.000285; //The pulse per inch travel is 3500 -> conversion from pulse to inch 

unsigned long prevTimer; //Start timer

const uint8_t PWM_SPEED = 100; //The speed at which the actuator are moving 
const uint8_t DELAY_TIME = 8000;

void setup() {

  pinMode(EXT_PIN, OUTPUT);
  pinMode(RET_PIN, OUTPUT);

  pinMode(PIN_R_ENABLE,OUTPUT);
  pinMode(PIN_L_ENABLE,OUTPUT);

  pinMode(ENCODER_A, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A),countPulse, RISING); //Interuppt the program to count the channel pulse from LOW -> HIGH

  Serial.begin(9600);
  prevTimer = millis();
}

void loop() {

  //Start by extending
  direction = 1;
  Extend();
  do {
    previousePosition = position; // Set old position
    updatePosition(); // Set new position from pulse
    
    if (millis() - prevTimer > 100) { //Print position ever 100ms
      prevTimer = millis();
      Serial.println(convertToInches(position));
    }
  }while(position != previousePosition); //Stops because pulse has not changed, therefore, not moving.

  Stop(); // Stop since fully extended 
  delay(1000);

  direction = 0; // Change direction to retracting
  Retract();
  do {
    previousePosition = position; // Set old position
    updatePosition(); // Set new position from pulse
    
    if (millis() - prevTimer > 100) { //Print position ever 100ms
      prevTimer = millis();
      Serial.println(convertToInches(position));
    }
  }while(position != previousePosition); //Stops because pulse has not changed, therefore, not moving.

  Stop(); // Stop since fully retracted 
  delay(1000);
}

//Extending the actuator 
void Extend(void) {
  Serial.println("Actuator is now extending");
  //Enable movement
  digitalWrite(PIN_R_ENABLE, HIGH);
  digitalWrite(PIN_L_ENABLE, HIGH);

  //Extend actuator
  analogWrite(EXT_PIN, PWM_SPEED);
  analogWrite(RET_PIN, 0);
}

//Retracting the actuator
void Retract(void) {
  Serial.println("Actuator is now retracting");
  //Enable movement
  digitalWrite(PIN_R_ENABLE, HIGH);
  digitalWrite(PIN_L_ENABLE, HIGH);

  //Retract actuator
  analogWrite(EXT_PIN, 0);
  analogWrite(RET_PIN, PWM_SPEED);
}

//Stopping the actuator
void Stop(void) {
  Serial.println("Actuator is now stopping");
  //Disable movement
  digitalWrite(PIN_R_ENABLE, LOW);
  digitalWrite(PIN_L_ENABLE, LOW);

  //Stop actuator
  analogWrite(EXT_PIN, 0);
  analogWrite(RET_PIN, 0);
}

//Count the pulse while dealing with false triggers
void countPulse(void){
  if((micros() - lastPulseTime) > trigDelay)
  {
    pulseCounter++;
    lastPulseTime = micros();
  }
}

//Update the position with pulseCounter based on extending or retracting
void updatePosition() {

  if(pulseCounter==0) return; // pulse did not change
  
  if(direction==1){
    position += pulseCounter;
    pulseCounter = 0;
  } else {
    position -= pulseCounter;
    pulseCounter = 0;
  }
}

//Convert position to inches
float convertToInches(long pos){
  return conNum*pos;
}