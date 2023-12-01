#include <Arduino.h>
#include "../lib/led/src/myNeoPixel.hpp"

// Objects
MyNeoPixel myNeoPixel;

// Global Variables

//  Prototyping

// Setup and Loop
void setup() {
  myNeoPixel.begin();
}

void loop() {
  myNeoPixel.setGreen(127);
  delay(500);
  myNeoPixel.setRed(127);
  delay(500);
  myNeoPixel.setBlue(127);
  delay(500);
}