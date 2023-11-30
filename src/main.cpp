#include <Arduino.h>
#include "../lib/dht/src/dht.hpp"
#include "../lib/api/src/api.hpp"
#include "../lib/wifi/src/wifi.hpp"
#include "../lib/sd/src/sdcustom.hpp"
#include "../lib/log/src/log.hpp"
#include "../lib/sensors/src/voltage_current.hpp"

/***********************************************************/
/****************** Global variable ************************/
/***********************************************************/
// Voltage and current
const int current_pin_12v = 32;
const int voltage_pin_12v = 33;

const int current_pin_5v = 34;
const int voltage_pin_5v = 35;

const int current_pin_solar = 36;
const int voltage_pin_solar = 39;

const int current_pin_battery = 25;
const int voltage_pin_battery = 26;

const int relay_pin = 27;

// State machine
enum state {
    INIT,
    CHECKING,
    FETCHING,
    SENDING,
    ERROR,
    SLEEP
};
state current_state = INIT;

// Debug
bool show_data = false;
const int sd_size = 32;

// DHT11
const int dh11_pin = 4;
enum dht_data { hum, temp };
float *dht_values;

// Deep sleep
unsigned long int start;
const int us_to_s_factor = 1000000;
int time_to_sleep = 5;
const int time_period = 60;

/***********************************************************/
/********************* Objects *****************************/
/***********************************************************/
wifi_connection wifi("Siri-al_killer", "15112001");
api_lib api;

SDCustom sd(sd_size);
myLogger logger(sd);

DHTSensor dht_sensor(dh11_pin);

CurrentSensor current_12v(current_pin_12v);
VoltageSensor voltage_12v(voltage_pin_12v);

// Can be used to store data in RTC memory during deep sleep
RTC_DATA_ATTR int log_file = logger.getLogFile();
// RTC_DATA_ATTR int old_log = logger.getOldLogFile();

/***********************************************************/
/********************* Prototype ***************************/
/***********************************************************/
void SerialEvent();
String get_wakeup_reason();

void setModemSleep();
void wakeModemSleep();

/***********************************************************/
/****************** Setup and Looop ************************/
/***********************************************************/
void setup() {
  Serial.begin(115200);
  Serial.println(log_file);

  // Set up logger
  logger.setLogFile(log_file);
  logger.setOldLogFile(0);
  logger.init(); 
  logger.setLevel(myLogger::level_t::DEBUG);

  logger.disableLoggingInSD();
  logger.enableLoggingInMonitor();
  
  // Set up wifi
  logger.debug("SETUP", "Wifi status : " + String(wifi.connect(10000)));

  wifi.connect(10000);
  api.setHost("https://api.thingspeak.com/update?api_key=72ZH5DA3WVKUD5R5");

  // Set up deep sleep
  esp_sleep_enable_timer_wakeup(time_to_sleep * us_to_s_factor);
  logger.info("SETUP", "Wakeup reason : " + get_wakeup_reason());

  // Setup sensors
  current_12v.setup();
  voltage_12v.setup();

  current_state = CHECKING;
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
  setModemSleep();
  SerialEvent();

  start = millis();

  switch (current_state) {
    case CHECKING:
      {
        logger.info("CHECKING", "CHECKING");
        if (!sd.isSDInserted()) {
          logger.disableLoggingInSD();
          logger.error("CHECKING", "SD Card is not inserted");
        }
        else {
          logger.enableLoggingInSD();
          logger.debug("CHECKING", "SD Card is inserted");
        }

        if (wifi.isConnected()) {
          logger.debug("CHECKING", "Wifi is connected");
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
          logger.debug("FETCHING", "DHT11 values are correct");
          current_state = SENDING;
        }
        break;
      }

    case SENDING:
      {
        logger.info("SENDING", "SENDING");
        float *data;

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
      
      // log_file = logger.getLogFile();
      // old_log = logger.getOldLogFile();
      log_file = 12;
      wifi.disconnect();
      esp_wifi_stop();
      esp_deep_sleep_start();
      // esp_light_sleep_start();
      break;
    default:
      logger.debug("DEFAULT", "DEFAULT state");
      break;
  }
}

/***********************************************************/
/********************* Functions ***************************/
/***********************************************************/
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

void setModemSleep() {
    WiFi.setSleep(true);
    if (!setCpuFrequencyMhz(40)){
        Serial2.println("Not valid frequency!");
    }
    // Use this if 40Mhz is not supported
    // setCpuFrequencyMhz(80);
}

void wakeModemSleep() {
    setCpuFrequencyMhz(240);
}