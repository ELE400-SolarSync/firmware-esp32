#include <Arduino.h>
#include "../lib/api/src/api.hpp"
#include "../lib/wifi/src/wifi.hpp"
#include "../lib/sd/src/sdcustom.hpp"
#include "../lib/log/src/log.hpp"
#include "../lib/sensors/src/voltage_current.hpp"
#include "../lib/sensors/src/dht.hpp"

#include "esp_wifi.h"

/***********************************************************/
/****************** Global variable ************************/
/***********************************************************/
// Voltage and current
const int current_pin_12v = A2;
const int voltage_pin_12v = A1;

const int current_pin_5v = A3;
const int voltage_pin_5v = A4;

const int current_pin_solar = A2;
const int voltage_pin_solar = A0;

const int current_pin_battery = A3;
const int voltage_pin_battery = A4;

const int relay_pin = 9;

enum v_c_data { v_12v, c_12v, v_5v, c_5v, v_solar, c_solar, v_battery, c_battery };
float *v_c_values;

enum pow_data { p_12v, p_5v, p_solar, p_battery };
float *pow_values;

float bat_level;

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

RTC_DATA_ATTR int log_info[2] = {0, 0};

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

CurrentSensor current_5v(current_pin_5v);
VoltageSensor volatage_5v(voltage_pin_5v);

CurrentSensor current_solar(current_pin_solar);
VoltageSensor voltage_solar(voltage_pin_solar);

CurrentSensor current_battery(current_pin_battery);
VoltageSensor voltage_battery(voltage_pin_battery);

/***********************************************************/
/********************* Prototype ***************************/
/***********************************************************/
void deepSleep(int sleepTime);
String get_wakeup_reason();
void SerialEvent();

/***********************************************************/
/****************** Setup and Looop ************************/
/***********************************************************/
void setup() {
  Serial.begin(115200);

  // Set up logger
  logger.setLogFile(log_info[0]);
  logger.setOldLogFile(log_info[1]);
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
  logger.info("SETUP", "Wakeup reason : " + get_wakeup_reason());

  // Setup sensors
  current_12v.setup();
  voltage_12v.setup();

  current_state = CHECKING;
}

void loop() 
{
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

        digitalWrite(relay_pin, LOW);
        delay(500);
        v_c_values[c_12v] = current_12v.readCurrent();
        v_c_values[v_12v] = voltage_12v.readVoltage();
        v_c_values[c_solar] = current_solar.readCurrent();
        v_c_values[v_solar] = voltage_solar.readVoltage();

        digitalWrite(relay_pin, HIGH);
        delay(500);
        v_c_values[v_5v] = volatage_5v.readVoltage();
        v_c_values[v_battery] = voltage_battery.readVoltage();
        v_c_values[c_5v] = current_5v.readCurrent();
        v_c_values[c_battery] = current_battery.readCurrent();

        pow_values[p_12v] = v_c_values[c_12v] * v_c_values[v_12v];
        pow_values[p_solar] = v_c_values[c_solar] * v_c_values[v_solar];
        pow_values[p_5v] = v_c_values[c_5v] * v_c_values[v_5v];
        pow_values[p_battery] = v_c_values[c_battery] * v_c_values[v_battery];

        bat_level = current_battery.getBatLevel(v_c_values[c_battery], v_c_values[v_battery]);
        
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
    {
      time_to_sleep = time_period - (start - millis()) * 1000;

      esp_sleep_enable_timer_wakeup(time_to_sleep * us_to_s_factor);
      logger.info("SLEEP", "Time to sleep: " + String(time_to_sleep) + " seconds");
      
      log_info[0] = logger.getLogFile();
      log_info[1] = logger.getOldLogFile();
      deepSleep(time_to_sleep);
      break;
    default:
      logger.debug("DEFAULT", "DEFAULT state");
      break;
    }
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

String get_wakeup_reason() {
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