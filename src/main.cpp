#include <Arduino.h>
#include "../lib/dht/src/dht.hpp"
#include "../lib/api/src/api.hpp"
#include "../lib/wifi/src/wifi.hpp"
#include "../lib/sd/src/sdcustom.hpp"
#include "../lib/log/src/log.hpp"
#include "../lib/sensors/src/voltage_current.hpp"

// Global Variables
const int debug = false;

// const int battery_current_pin = A6;
// const int battery_voltage_pin = A7;

// const int solar_current_pin = A2;
// const int solar_voltage_pin = A3;

const int current_pin_5v = A0;
const int voltage_pin_5v = A1;

// const int current_pin_12v = A0;
// const int voltage_pin_12v = A1;

const int current_pin_12v = 32;
const int voltage_pin_12v = 33;

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

const int dh11_pin = 4;             /* Pin where the DHT11 is connected */
enum dht_data { hum, temp };        /* Indexes for the DHT11 values */
float *dht_values;                  /* Array to store the DHT11 values */

const int us_to_s_factor = 1000000;  /* Conversion factor for micro seconds to seconds */
int time_to_sleep = 5;        /* Time ESP32 will go to sleep (in seconds) */
const int time_period = 60;

// Objects
DHTSensor dht_sensor(dh11_pin);
SDCustom sd(32);
wifi_connection wifi("LakeLaogai", "thereisnowifiinbasingse");
api_lib api;
myLogger logger(sd);

// CurrentSensor current_battery(battery_current_pin);
// VoltageSensor voltage_battery(battery_voltage_pin);

// CurrentSensor current_solar(solar_current_pin);
// VoltageSensor voltage_solar(solar_voltage_pin);

CurrentSensor current_5v(current_pin_5v);
VoltageSensor voltage_5v(voltage_pin_5v);

// Can be used to store data in RTC memory during deep sleep
RTC_DATA_ATTR int log_file = logger.getLogFile();
RTC_DATA_ATTR int old_log = logger.getOldLogFile();

// CurrentSensor current_battery(battery_current_pin);
// VoltageSensor voltage_battery(battery_voltage_pin);

CurrentSensor current_solar(solar_current_pin);
VoltageSensor voltage_solar(solar_voltage_pin);

CurrentSensor current_5v(current_pin_5v);
VoltageSensor voltage_5v(voltage_pin_5v);

CurrentSensor current_12v(current_pin_12v);
VoltageSensor voltage_12v(voltage_pin_12v);

// Prototypes
void SerialEvent();
String get_wakeup_reason();

// Setup and Loop
void setup() {
  Serial.begin(115200);

  logger.setLogFile(log_file);
  logger.setOldLogFile(old_log);
  logger.init(); 
  
  logger.debug("SETUP", "Wifi status : " + String(wifi.connect(10000)));
  
  logger.disableLoggingInSD();
  logger.enableLoggingInMonitor();

  wifi.connect(10000);
  api.setHost("https://api.thingspeak.com/update?api_key=72ZH5DA3WVKUD5R5");

  // Set up deep sleep
  esp_sleep_enable_timer_wakeup(time_to_sleep * us_to_s_factor);
  logger.info("SETUP", "Wakeup reason : " + get_wakeup_reason());

  // current_battery.setup();
  // voltage_battery.setup();

  // current_solar.setup();
  // voltage_solar.setup();

  current_5v.setup();
  voltage_5v.setup();

  // current_12v.setup();
  // voltage_12v.setup();
}

// pins choice:
// A0 -> voltage solar cell
// A1 -> voltage 12V
// A2 -> relay 1
// A3 -> relay 2
// A4 -> relay 3

// relay choice
    // 1 : voltage bat/5v
    // 2 : current solar cell/12V
    // 3 : current bat/5V

// step 1:
// switch for reading solar cell/battery
// read solar cell
// read battery

// step 2:
// switch for reading 12V and 5V
// read 5V
// read 12V

void loop() {
  SerialEvent();

  start = millis();

  switch (current_state) {
      {
        logger.info("CHECKING", "CHECKING");
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
        logger.info("FETCHING", "FETCHING");
        dht_values = dht_sensor.getValues();
        
        if (dht_sensor.isCorrect_values(dht_values)) {
          logger.error("FETCHING", "DHT11 values are incorrect");
          current_state = ERROR;
        }
        else {
          logger.info("FETCHING", "DHT11 values are correct");
          current_state = SENDING;
        }
        break;
      }

    case SENDING:
      {
        logger.info("SENDING", "SENDING");
        data[0] = dht_values[temp];
        data[1] = dht_values[hum];
        api_lib::response res;
        res = api.sendAll(data, sizeof(data)/sizeof(float));
        if (res.code == 200) {
          logger.debug("SENDING", "Return message : " + res.data);
          current_state = SLEEP;
        }
        break;
      }

    case ERROR:
      {
        logger.info("ERROR", "ERROR state");
        current_state = SLEEP;
        break;
      }

    case SLEEP:
      time_to_sleep = time_period - (start - millis()) * 1000;

      esp_sleep_enable_timer_wakeup(time_to_sleep * us_to_s_factor);
      logger.info("SLEEP", "Time to sleep: " + String(time_to_sleep) + " seconds");
      
      esp_deep_sleep_start();
      // esp_light_sleep_start();

      // delay(time_to_sleep * 1000);
      current_state = CHECKING;
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