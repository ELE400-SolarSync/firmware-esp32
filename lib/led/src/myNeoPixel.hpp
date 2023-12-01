#ifndef MYNEOPIXEL_HPP
#define MYNEOPIXEL_HPP

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class MyNeoPixel {
    private:
        Adafruit_NeoPixel strip;

    public:
        MyNeoPixel();
        void begin();
        void setGreen(int brightness);
        void setRed(int brightness);
        void setBlue(int brightness);
};

#endif