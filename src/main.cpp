#include <Arduino.h>
#include "../lib/api/src/api.hpp"
#include "../lib/wifi/src/wifi.hpp"
#include "../lib/log/src/log.hpp"

// Objects
wifi_connection wifi("Siri-al_killer", "15112001");
api_lib api;
myLogger logger("log.txt", "/log/");

// Global Variables
const int debug = false;

int connection_status;

const int LED = 2;  

float fake_data = 0.;

// Global variables
enum state {
    INIT,
    CHECKING,
    FETCHING,
    SENDING,
    SLEEP,
    RECO
};

state current_state = INIT;

bool check, finish, sent = false;

const int us_to_s_factor = 1000000;  /* Conversion factor for micro seconds to seconds */
const int time_to_sleep = 5;        /* Time ESP32 will go to sleep (in seconds) */

// Can be used to store data in RTC memory during deep sleep
// RTC_DATA_ATTR int bootCount = 0;

// Prototypes
void SerialEvent();
String get_wakeup_reason();

// Setup and Loop
void setup() {
  Serial.begin(115200);

  // Set up logger
  logger.setLevel(logger.DEBUG);

  // Set up LED
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);

  // Set up state machine
  current_state = CHECKING;

  // Set up wifi
  connection_status = wifi.connect(10000);

  // Set up api
  api.setHost("https://api.thingspeak.com/update?api_key=72ZH5DA3WVKUD5R5");

  // Set up deep sleep
  esp_sleep_enable_timer_wakeup(time_to_sleep * us_to_s_factor);
}

void loop() {
  digitalWrite(LED, HIGH);
  SerialEvent();

  switch (current_state) {
    case CHECKING:
      {check = true;

      if (check) {
        current_state = FETCHING;
      } else {
        current_state = SLEEP;
      }

      check = false;

      break;}
    case FETCHING:
      {finish = true;

      if (finish) {
        current_state = SENDING;
      } else {
        current_state = SLEEP;
      }

      finish = false;

      break;}
    case SENDING: {
      if(!wifi.getStatus()) {
        logger.error("main", "Wifi not connected");
        current_state = RECO; 
        break;
      }
      int result = api.getCode("&field1="+String(fake_data));
      fake_data += 1.;
      logger.debug("main", "Data sent, return Code : " + String(result));
      delay(15000);  

      sent = true;
      if (sent) {
        current_state = CHECKING;
        sent = false;
      } else {
        current_state = SLEEP;
      }
      sent = false;

      break;
      }
    case RECO: {
      logger.info("main", "Reconnecting to wifi");
      wifi.connect(10000);
      unsigned long start = millis();
      while(millis()<start+10000){
        ;
      }
      if(wifi.getStatus()){
        current_state = CHECKING;
      }
      else{
        current_state = RECO;
      }
      break;
    }
    case SLEEP: {
      logger.info("main", "Going to sleep");
      Serial.flush(); 
      esp_deep_sleep_start();
      break;
    }
    default:
      logger.info("main", "Default case");
      delay(5000);
      break;
  }
}


// Functions
void SerialEvent() {
  while (Serial.available()) {
    String inChar = Serial.readString();

    if(inChar.indexOf("level")!=-1 && inChar.indexOf("level")<inChar.length()){
      if(inChar.endsWith("d")){
        logger.setLevel(logger.INFO);
        logger.info("UART", "Set log level to DEBUG");
        logger.setLevel(logger.DEBUG);
      }
      else if(inChar.endsWith("i")){
        logger.setLevel(logger.INFO);
        logger.info("UART", "Set log level to INFO");
        logger.setLevel(logger.INFO);
      }
      else if(inChar.endsWith("w")){
        logger.setLevel(logger.INFO);
        logger.info("UART", "Set log level to WARNING");
        logger.setLevel(logger.WARN);
      }
      else if(inChar.endsWith("e")){
        logger.setLevel(logger.INFO);
        logger.info("UART", "Set log level to ERROR");
        logger.setLevel(logger.ERROR);
      }
      else{
        logger.setLevel(logger.INFO);
        logger.info("UART", "Unknown log level");
        logger.setLevel(logger.getLevel());
      }
    }

    else if (inChar == "hello") {
      logger.info("UART", "hello, world!");
    }
    else if (inChar == "wifi_ip") {
      logger.info("UART", "IP Address: "+wifi.getIP());
    }
    else {
      Serial.println("Command not found");
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