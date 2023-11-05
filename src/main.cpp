#include <Arduino.h>
#include "../lib/api/src/api.hpp"
#include "../lib/wifi/src/wifi.hpp"

wifi_connection wifi("LakeLaogai", "thereisnowifiinbasingse");

api_lib api;

int connection_status;

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
  Serial.println("Sending request");
  float data[] = {1.0, 2.0, 3.0};
  api.sendAmp(data, sizeof(data)/sizeof(data[0]));
  // Serial.println(result);
  delay(1000);
}