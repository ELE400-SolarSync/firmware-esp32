#include <Arduino.h>
#include "../lib/api/src/api.hpp"
#include "../lib/wifi/src/wifi.hpp"
#include "../lib/sd/src/sdcustom.hpp"
#include "../lib/log/src/log.hpp"

// Global Variables
const int debug = false;

enum state {
    INIT,
    CHECKING,
    FETCHING,
    SENDING,
    SLEEP
};

state current_state = INIT;

bool check, finish, sent = false;

const int us_to_s_factor = 1000000;  /* Conversion factor for micro seconds to seconds */
const int time_to_sleep = 5;        /* Time ESP32 will go to sleep (in seconds) */

int log_test = 0;

// Can be used to store data in RTC memory during deep sleep
// RTC_DATA_ATTR int bootCount = 0;

// Objects
SDCustom sd(32);
wifi_connection wifi("LakeLaogai", "thereisnowifiinbasingse");
api_lib api;
myLogger logger(sd);

// Prototypes
void SerialEvent();
String get_wakeup_reason();

// Setup and Loop
void setup() {
  Serial.begin(115200);

  logger.init(); 

  // Set up deep sleep
  esp_sleep_enable_timer_wakeup(time_to_sleep * us_to_s_factor);
}

void loop() {
  SerialEvent();

  logger.info("main", "test " + String(log_test));
  log_test++;
  delay(1000);
}

// Functions
void SerialEvent() {
  while (Serial.available()) {
    String inChar = Serial.readString();
    if (inChar.indexOf("date") != -1) {
      Serial.println("lol");
    }
    // if (inChar.indexOf("terminal") != -1) {
    //   if (inChar.indexOf("enable") != -1)
    //     logger.enableLoggingInMonitor();
    //   else if (inChar.indexOf("disable") != -1)
    //     logger.disableLoggingInMonitor();
    // }
  }
}

/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
String get_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  String reason;

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : reason = "Wakeup caused by external signal using RTC_IO"; break;
    case ESP_SLEEP_WAKEUP_EXT1 : reason = "Wakeup caused by external signal using RTC_CNTL"; break;
    case ESP_SLEEP_WAKEUP_TIMER : reason = "Wakeup caused by timer"; break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : reason = "Wakeup caused by touchpad"; break;
    case ESP_SLEEP_WAKEUP_ULP : reason = "Wakeup caused by ULP program"; break;
    default : reason = "Wakeup was not caused by deep sleep: %d\n",wakeup_reason; break;
  }

  return reason;
}
