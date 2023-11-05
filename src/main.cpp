#include <Arduino.h>
#include "../lib/api/api.hpp"
#include "../lib/wifi/wifi.hpp"

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
  String result = api.get("&field1=0");
  Serial.println(result);
  delay(15000);
}