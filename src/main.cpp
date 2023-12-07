#include <Arduino.h>
#include "../lib/api/src/api.hpp"
#include "../lib/wifi/src/wifi.hpp"
#include "../lib/sd/src/sdcustom.hpp"
#include "../lib/log/src/log.hpp"
#include "../lib/sensors/src/voltage_current.hpp"
#include "../lib/sensors/src/dht.hpp"

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
float v_c_values[8];

enum pow_data { p_12v, p_5v, p_solar, p_battery };
float pow_values[4];

float bat_level;

// API
float all_data[15];
api_lib::response res;

// Relay for the output
const int relay_output_pin = 8;

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

enum errors_t {
  NONE,
  SD_NOT_INSERTED,
  WIFI_NOT_CONNECTED,
  DHT11_VALUES_INCORRECT,
  FETCHING_ERROR
};

error_t current_error = NONE;
float correction = 0;

// Debug
bool show_data = false;
const int sd_size = 32;

// DHT11
const int dh11_pin = 10;
enum dht_data { hum, temp };
float dht_values[2];
unsigned long start_dht;

// Deep sleep
unsigned long start;
const int s_to_us_factor = 1000000;
int time_to_sleep = 5;
const int time_period = 15;

RTC_DATA_ATTR int log_info[2] = {0, 0};

/***********************************************************/
/********************* Objects *****************************/
/***********************************************************/
wifi_connection wifi("", "");
api_lib api;

SDCustom sd(sd_size);
myLogger logger(sd);

DHTSensor dht_sensor(dh11_pin);

CurrentSensor current_12v(current_pin_12v);
VoltageSensor voltage_12v(voltage_pin_12v);

CurrentSensor current_5v(current_pin_5v);
VoltageSensor voltage_5v(voltage_pin_5v);

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

  pinMode(relay_pin, OUTPUT);
  pinMode(relay_output_pin, OUTPUT);

  digitalWrite(relay_output_pin, HIGH);

  // Set up logger
  logger.setOldLogFile(0);
  logger.init(); 
  logger.setLevel(myLogger::level_t::DEBUG);

  logger.enableLoggingInSD();
  logger.enableLoggingInMonitor();
  
  // Set up wifi
  logger.debug("SETUP", "Wifi status : " + String(wifi.connect(30)));

  // Setup sensors
  current_12v.setup();
  voltage_12v.setup();

  current_state = CHECKING;
}

void loop() 
{
  SerialEvent();

  switch (current_state) {
    case CHECKING:
      start = millis();

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
        current_error = WIFI_NOT_CONNECTED;
      }
      break;
    

    case FETCHING:
      logger.info("FETCHING", "FETCHING");

      dht_values[temp] = dht_sensor.getTemperature();
      dht_values[hum] = dht_sensor.getHumidity();

      start_dht = millis();

      while((isnan(dht_values[temp]) || isnan(dht_values[hum])) && (millis() - start_dht < 10000)) {
        dht_values[temp] = dht_sensor.getTemperature();
        dht_values[hum] = dht_sensor.getHumidity();
      }

      dht_values[temp] = isnan(dht_values[temp]) ? -1 : dht_values[temp];
      dht_values[hum] = isnan(dht_values[hum]) ? -1 : dht_values[hum];

      // solar CS offset: 2.49V
      // battery CS offset: 2.48V
      // 12V CS offset: 2.50V 
      // 5V CS offset: 2.50V

      digitalWrite(relay_pin, LOW);
      delay(4000);
      v_c_values[v_battery] = voltage_battery.readVoltage();
      v_c_values[c_battery] = current_battery.readCurrent();
      v_c_values[v_solar] = voltage_solar.readVoltage();
      v_c_values[c_solar] = current_solar.readCurrent();

      digitalWrite(relay_pin, HIGH);
      delay(4000);
      v_c_values[v_5v] = voltage_5v.readVoltage();
      v_c_values[c_5v] = current_5v.readCurrent();
      v_c_values[v_12v] = voltage_12v.readVoltage();
      v_c_values[c_12v] = current_12v.readCurrent();

      // voltage correction
      correction = v_c_values[v_5v] / 5.0f;

      v_c_values[v_battery] = VoltageSensor::CorrectReading(correction, v_c_values[v_battery]);
      v_c_values[c_battery] = CurrentSensor::CorrectReading(correction, v_c_values[c_battery]);
      v_c_values[v_solar] = VoltageSensor::CorrectReading(correction, v_c_values[v_solar]);
      v_c_values[c_solar] = CurrentSensor::CorrectReading(correction, v_c_values[c_solar]);
      v_c_values[v_5v] = VoltageSensor::CorrectReading(correction, v_c_values[v_5v]);
      v_c_values[c_5v] = CurrentSensor::CorrectReading(correction, v_c_values[c_5v]);
      v_c_values[v_12v] = VoltageSensor::CorrectReading(correction, v_c_values[v_12v]);
      v_c_values[c_12v] = CurrentSensor::CorrectReading(correction, v_c_values[c_12v]);


      pow_values[p_12v] = v_c_values[c_12v] * v_c_values[v_12v];
      pow_values[p_solar] = v_c_values[c_solar] * v_c_values[v_solar];
      pow_values[p_5v] = v_c_values[c_5v] * v_c_values[v_5v];
      pow_values[p_battery] = v_c_values[c_battery] * v_c_values[v_battery];

      bat_level = current_battery.getBatLevel(v_c_values[v_battery]);

      if (dht_sensor.isCorrect_values(dht_values)) {
        logger.debug("FETCHING", "DHT11 values are correct");
      }
      else {
        logger.error("FETCHING", "DHT11 values are incorrect");
      }
      current_state = SENDING;
      break;

    case SENDING:
      logger.info("SENDING", "SENDING");

      if(show_data){
        logger.info("Values Start", "-------------------------------");

        logger.info("DHT11", String(dht_values[temp]) + "C " + String(dht_values[hum]) + "%");

        logger.info("12V", String(v_c_values[c_12v]) + "A " + String(v_c_values[v_12v]) + "V");

        logger.info("5V", String(v_c_values[c_5v]) + "A " + String(v_c_values[v_5v]) + "V");

        logger.info("Solar",  String(v_c_values[c_solar]) + "A " + String(v_c_values[v_solar]) + "V");

        logger.info("Battery", String(v_c_values[c_battery]) + "A " + String(v_c_values[v_battery]) + "V");

        logger.info("Values End", "-------------------------------");
      }

      api.clearJson();
      api.createJson(dht_values[temp], pow_values[p_solar], pow_values[p_battery], pow_values[p_5v], pow_values[p_12v], bat_level, false);

      res = api.getResponse();
      if (res.code == 204) {
        logger.debug("SENDING", "Return message : " + res.data);
        current_state = SLEEP;
      }
      else {
        logger.error("SENDING", "API call failed : " + String(res.code));
        current_state = SLEEP;
      }

      break;
      

    case ERROR:
      if(current_error == WIFI_NOT_CONNECTED) {
        logger.error("ERROR", "Wifi is not connected");
        current_state = FETCHING;
      }
      else if(current_error == DHT11_VALUES_INCORRECT) {
        logger.error("ERROR", "DHT11 values are incorrect");
        current_state = SLEEP;
      }
      else if(current_error == FETCHING_ERROR) {
        logger.error("ERROR", "Error while fetching data");
        current_state = SLEEP;
      }
      else {
        logger.error("ERROR", "Unknown error");
      }
      break;
      

    case SLEEP:
    logger.info("SLEEP", "SLEEP");
      time_to_sleep = time_period - (millis() - start) / 1000;
      
      log_info[0] = logger.getLogFile();
      log_info[1] = logger.getOldLogFile();
      // deepSleep(time_to_sleep);
      delay(1000);
      current_state = CHECKING;
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

    if(inChar.indexOf("show_data") != -1){
      show_data = !show_data;
    }

    if(inChar.indexOf("output") != -1){
      if(inChar.indexOf("on") != -1){
        logger.debug("SerialEvent", "Output on");
        digitalWrite(relay_output_pin, HIGH);
      }
      else if(inChar.indexOf("off") != -1){
        logger.debug("SerialEvent", "Output off");
        digitalWrite(relay_output_pin, LOW);
      }
    }

    if(inChar.indexOf("logging") != -1){
      if(inChar.indexOf("sd") != -1){
        if(inChar.indexOf("on") != -1){
          logger.enableLoggingInSD();
        }
        else if(inChar.indexOf("off") != -1){
          logger.disableLoggingInSD();
        }
      }
      else if(inChar.indexOf("serial") != -1){
        if(inChar.indexOf("on") != -1){
          logger.enableLoggingInMonitor();
        }
        else if(inChar.indexOf("off") != -1){
          logger.disableLoggingInMonitor();
        }
      }
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
  esp_sleep_enable_timer_wakeup(sleepTime * s_to_us_factor);

  // Enter deep sleep
  esp_deep_sleep_start();
}