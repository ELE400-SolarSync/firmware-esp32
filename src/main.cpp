#include <Arduino.h>
#include "../lib/api/src/api.hpp"
#include "../lib/wifi/src/wifi.hpp"

/***********************************************************/
/****************** Global variable ************************/
/***********************************************************/

// Deep sleep
unsigned long int start;
const int us_to_s_factor = 1000000;
int time_to_sleep = 5;
const int time_period = 60;

RTC_DATA_ATTR int log_info[2] = {0, 0};

/***********************************************************/
/********************* Objects *****************************/
/***********************************************************/
wifi_connection wifi("Siri-al_killer", "15112001");
api_lib api;

/***********************************************************/
/********************* Prototype ***************************/
/***********************************************************/
void deepSleep(int sleepTime);
String get_wakeup_reason();

/***********************************************************/
/****************** Setup and Looop ************************/
/***********************************************************/
HTTPClient http;
void setup() {
  Serial.begin(115200);
  Serial.println(get_wakeup_reason());

  wifi.connect(20);

  Serial.print("Wifi  status: ");
  Serial.println(wifi.getStatus());

  api.setHost("https://api.thingspeak.com/update?api_key=72ZH5DA3WVKUD5R5");
}

void loop() {
  // float data[1] = {1.0};
  // api_lib::response res = api.sendAll(data, 1);
  // Serial.println(res.data);

  log_info[0] = log_info[0] + 1;
  log_info[1] = log_info[1] + 2;
  Serial.println("Boot number: " + String(log_info[0]) + ", " + String(log_info[1]));

  deepSleep(15);
}

/***********************************************************/
/********************* Functions ***************************/
/***********************************************************/
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

void deepSleep(int sleepTime) {
  // Disconnect from Wi-Fi
  wifi.disconnect();
  wifi.setModeOff();

  // Disable Bluetooth
  btStop();

  // Set sleep mode
  esp_sleep_enable_timer_wakeup(sleepTime * us_to_s_factor);

  // Enter deep sleep
  esp_deep_sleep_start();
}