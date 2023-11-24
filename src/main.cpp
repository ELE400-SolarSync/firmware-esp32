#include <Arduino.h>

const int sensorPin = A0;

void setup() {
  pinMode(sensorPin, INPUT);
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  delay(1000);
}