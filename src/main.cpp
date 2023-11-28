#include <Arduino.h>
#include "../lib/api/src/api.hpp"
#include "../lib/wifi/src/wifi.hpp"
#include "../lib/sd/src/sdcustom.hpp"
#include "../lib/log/src/log.hpp"
#include "../lib/sensors/src/voltage_current.hpp"

// Global Variables
const int debug = false;

// const int battery_current_pin = A6;
// const int battery_voltage_pin = A7;

const int solar_current_pin = A2;
const int solar_voltage_pin = A3;

const int current_pin_5v = A4;
const int voltage_pin_5v = A5;

const int current_pin_12v = A0;
const int voltage_pin_12v = A1;

// CurrentSensor current_battery(battery_current_pin);
// VoltageSensor voltage_battery(battery_voltage_pin);

CurrentSensor current_solar(solar_current_pin);
VoltageSensor voltage_solar(solar_voltage_pin);

CurrentSensor current_5v(current_pin_5v);
VoltageSensor voltage_5v(voltage_pin_5v);

CurrentSensor current_12v(current_pin_12v);
VoltageSensor voltage_12v(voltage_pin_12v);

enum state {
    INIT,
    CHECKING,
    FETCHING,
    SENDING,
    ERROR,
    SLEEP
};

state current_state = INIT;

unsigned long int start;

float data[3] = {1.0, 2.0, 3.0};

bool show_data = false;

const int us_to_s_factor = 1000000;  /* Conversion factor for micro seconds to seconds */
int time_to_sleep = 5;        /* Time ESP32 will go to sleep (in seconds) */
const int time_period = 60;

// Objects
SDCustom sd(32);
wifi_connection wifi("LakeLaogai", "thereisnowifiinbasingse");
api_lib api;
myLogger logger(sd);

// Can be used to store data in RTC memory during deep sleep
RTC_DATA_ATTR int log_file = logger.getLogFile();
RTC_DATA_ATTR int old_log = logger.getOldLogFile();

// Prototypes
void SerialEvent();
String get_wakeup_reason();

// Setup and Loop
void setup() {
  Serial.begin(115200);

  logger.setLogFile(log_file);
  logger.setOldLogFile(old_log);
  logger.init(); 

  logger.disableLoggingInSD();
  logger.enableLoggingInMonitor();


  logger.info("SETUP", "Wifi status : " + String(wifi.connect(10000)));
  api.setHost("https://api.thingspeak.com/update?api_key=72ZH5DA3WVKUD5R5");

  // Set up deep sleep
  esp_sleep_enable_timer_wakeup(time_to_sleep * us_to_s_factor);
  logger.info("SETUP", "Wakeup reason : " + get_wakeup_reason());

  current_state = CHECKING;

  // current_battery.setup();
  // voltage_battery.setup();

  current_solar.setup();
  voltage_solar.setup();

  current_5v.setup();
  voltage_5v.setup();

  current_12v.setup();
  voltage_12v.setup();
}

void loop() {
  SerialEvent();

  start = millis();

  switch (current_state) {
    case CHECKING:
      {
        logger.info("main", "CHECKING");
        if (!sd.isSDInserted()) {
          logger.disableLoggingInSD();
        }
        else {
          logger.enableLoggingInSD();
          logger.warning("CHECKING", "SD Card is inserted");
        }
        if (wifi.isConnected()) {
          current_state = FETCHING;
        }
        else {
          logger.error("CHECKING", "Wifi is not connected");
          current_state = ERROR;
        }
        break;
      }

    case FETCHING:
      {
        logger.info("main", "FETCHING");
        if(show_data){
          myLogger::level_t level = logger.getLevel();
          logger.setLevel(myLogger::DEBUG);
          logger.debug("DATA", "Humidity : ");
          logger.setLevel(level);
      }
        current_state = SENDING;
        break;
      }

    case SENDING:
      {
        logger.info("main", "SENDING");
        if (api.sendAll(data, sizeof(data)/sizeof(float)) == 200) {
          current_state = SLEEP;
        }
        break;
      }

    case ERROR:
      {
        logger.info("ERROR", "ERROR state");
        break;
      }

    case SLEEP:
      time_to_sleep = time_period - (start - millis()) * 1000;

      esp_sleep_enable_timer_wakeup(time_to_sleep * us_to_s_factor);
      logger.info("SLEEP", "Time to sleep: " + String(time_to_sleep) + " seconds");
      esp_deep_sleep_start();
      // esp_light_sleep_start();

      delay(time_to_sleep * 1000);
      break;
    default:
      logger.info("DEFAULT", "DEFAULT state");
      break;
  }
}

// Functions
void SerialEvent() {
  while (Serial.available()) {
    String inChar = Serial.readString();
    if (inChar.indexOf("date") != -1) {
      Serial.println("lol");
    }
    if(inChar.indexOf("show_data") != -1){
      show_data = !show_data;
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