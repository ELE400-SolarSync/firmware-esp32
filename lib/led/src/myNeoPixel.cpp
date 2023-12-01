#include "myNeoPixel.hpp"

MyNeoPixel::MyNeoPixel() : strip(1, 8, NEO_GRB + NEO_KHZ800) {}

void MyNeoPixel::begin() {
  strip.begin();
}

void MyNeoPixel::setGreen(int brightness) {
  strip.setPixelColor(0, strip.Color(0, brightness, 0));
  strip.show();
}

void MyNeoPixel::setRed(int brightness) {
  strip.setPixelColor(0, strip.Color(brightness, 0, 0));
  strip.show();
}

void MyNeoPixel::setBlue(int brightness) {
  strip.setPixelColor(0, strip.Color(0, 0, brightness));
  strip.show();
}