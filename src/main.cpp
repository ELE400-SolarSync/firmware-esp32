#include <Arduino.h>
#include "../lib/log/log.hpp"
#include "esp_log.h"

// Objects
myLogger logger("log.txt", "log/", myLogger::ERROR);

// Global variables
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

// Can be used to store data in RTC memory during deep sleep
// RTC_DATA_ATTR int bootCount = 0;

int i = 0;

// Prototypes
void SerialEvent();
String get_wakeup_reason();

// Setup and Loop
void setup() {
  Serial.begin(115200);
  current_state = CHECKING;

  // Set up deep sleep
  esp_sleep_enable_timer_wakeup(time_to_sleep * us_to_s_factor);
  logger.setLevel(myLogger::DEBUG);
}

void loop() {
  SerialEvent();

  switch (current_state) {
    case CHECKING:
      Serial.println("CHECKING");
      delay(5000);

      check = true;

      if (check) {
        current_state = FETCHING;
      } else {
        current_state = SLEEP;
      }

      break;
    case FETCHING:
      Serial.println("FETCHING");
      delay(5000);

      finish = true;

      if (finish) {
        current_state = SENDING;
      } else {
        current_state = SLEEP;
      }

      break;
    case SENDING:
      Serial.println("SENDING");
      delay(5000);

      sent = true;

      if (sent) {
        current_state = CHECKING;
      } else {
        current_state = SLEEP;
      }

      break;
    case SLEEP:
      Serial.println("SLEEP");
      Serial.flush(); 
      esp_deep_sleep_start();
      break;
    default:
      Serial.println("FAIL");
      delay(5000);
      break;
  }
}


// Functions
void SerialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    switch (inChar) {
      case 'c':
        current_state = CHECKING;
        break;
      case 'f':
        current_state = FETCHING;
        break;
      case 'a':
        current_state = SENDING;
        break;
      case 's':
        current_state = SLEEP;
        Serial.println("ESP going to sleep for %s seconds" + String(time_to_sleep));
        Serial.flush(); 
        esp_deep_sleep_start();
        break;
      case 'l':
        logger.enableLoggingInMonitor();
        Serial.println("Logging enabled");
        break;
      case 'L':
        Serial.println("Logging disabled");
        logger.disableLoggingInMonitor();
        break;
      default:
        break;
    }
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