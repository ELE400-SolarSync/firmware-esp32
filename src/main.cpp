#include <Arduino.h>
#include "../lib/api/src/api.hpp"
#include "../lib/wifi/src/wifi.hpp"

// Objects
wifi_connection wifi("LakeLaogai", "thereisnowifiinbasingse");
api_lib api;

// Global Variables
const int debug = false;

int connection_status;


//  Prototyping
void SerialEvent();

// Setup and Loop
void setup() {
  Serial.begin(115200);

  while(!Serial) {
    ;
  }
  delay(1000);
  connection_status = wifi.connect(10000);

  api.setHost("https://api.thingspeak.com/update?api_key=72ZH5DA3WVKUD5R5");
}

void loop() {
  // polling for serial input
  SerialEvent();

  if (debug) {
    Serial.print("Connection status: ");
    Serial.println(wifi.getStatus());
    Serial.print("IP Address: ");
    Serial.println(wifi.getIP());
  }

  float data[] = {1.0, 2.0, 3.0};
  api.sendAmp(data, sizeof(data)/sizeof(data[0]));
  delay(1000);
}

// Functions
void SerialEvent() {
  while (Serial.available()) {
    String inChar = Serial.readString();

    if (inChar == "hello") {
      Serial.println("hello, world!");
    }
    else if (inChar == "wifi_ip") {
      Serial.print("IP Address: ");
      Serial.println(wifi.getIP());
    }
    else {
      Serial.println("Command not found");
    }    
  }
} 