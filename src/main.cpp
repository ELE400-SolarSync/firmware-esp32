#include <Arduino.h>
#include "../lib/dht/src/dht.hpp"

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

const int dh11_pin = 4;             /* Pin where the DHT11 is connected */
enum dht_data { hum, temp };        /* Indexes for the DHT11 values */
float *dht_values;                  /* Array to store the DHT11 values */

const int us_to_s_factor = 1000000;  /* Conversion factor for micro seconds to seconds */
const int time_to_sleep = 5;        /* Time ESP32 will go to sleep (in seconds) */

// Can be used to store data in RTC memory during deep sleep
// RTC_DATA_ATTR int bootCount = 0;

// Objects
DHTSensor dht_sensor(dh11_pin, DHT11);

// Prototypes
void SerialEvent();
String get_wakeup_reason();

// Setup and Loop
void setup() {
  Serial.begin(115200);

  current_state = CHECKING;

  // Set up deep sleep
  esp_sleep_enable_timer_wakeup(time_to_sleep * us_to_s_factor);
}

void loop() {
  SerialEvent();

  dht_values = dht_sensor.getValues();

  Serial.print("Humidity: ");
  Serial.println(dht_values[hum]);
  Serial.print("Temperature: ");
  Serial.println(dht_values[temp]);

  delay(2000);
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