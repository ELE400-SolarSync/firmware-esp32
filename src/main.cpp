#include <Arduino.h>
#include "../lib/api/src/api.hpp"
#include "../lib/wifi/src/wifi.hpp"
#include "../lib/sd/src/sdcustom.hpp"

// Global Variables
const int debug = false;

// Objects
SDCustom sd;
wifi_connection wifi("LakeLaogai", "thereisnowifiinbasingse");
api_lib api;

//  Prototyping

// Setup and Loop

void setup() {
  Serial.begin(115200);
  
  // wifi.connect(10000);
  sd.begin();

  sd.deleteFile("esp32.txt");
  sd.writeFile("esp32.txt", "Hello World");
  sd.writeFile("esp32.txt", "it's me again");
}

void loop() {
  Serial.println(sd.readFile("esp32.txt"));
  // Serial.println(api.getDate("America/Montreal/"));
  while(1){};
}

// Functions