#include <Arduino.h>
#include "../lib/api/api.hpp"
#include "../lib/wifi/wifi.hpp"

wifi_connection wifi("LakeLaogai", "thereisnowifiinbasingse");

int connection_status;

void setup() {
  Serial.begin(115200);

  while(!Serial) {
    ;
  }
  delay(1000);
  connection_status = wifi.connect(10000);
}

void loop() {
  Serial.println(wifi.getIP());
  delay(1000);
}