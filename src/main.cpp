#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Objects

// Global Variables
const char* ssid = "LakeLaogai";
const char* password = "thereisnowifiinbasingse";

const char* host = "solarsync.azure-devices.net";
const char* device_id = "esp32hub";
const char* sas_token = "SharedAccessSignature sr=SolarSync.azure-devices.net%2Fdevices%2Fesp32hub&sig=qTW1FcZWh8M1kzhDTHFCAmqY3SPP%2FD0reP6eHn0dyhE%3D&se=1701753692";


// Azure IoT Hub endpoint for posting messages
String url = String("https://") + host + "/devices/" + device_id + "/messages/events?api-version=2018-06-30";

//  Prototyping

// Setup and Loop
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  StaticJsonDocument<256> jsonDoc;
  jsonDoc["Temperature"] = 25;
  jsonDoc["Puissance"] = 25;
  jsonDoc["Niveau batterie"] = 25;
  jsonDoc["niveauBatterieAlert"] = true;

  String jsonBuffer;
  serializeJson(jsonDoc, jsonBuffer);

  // Send HTTP POST request
  HTTPClient httpClient;
  // httpClient.setTimeout(10000);
  if(!httpClient.begin(url)){
    Serial.println("Could not connect to Azure");
    return;
  }
  httpClient.addHeader("Content-Type", "application/json");
  httpClient.addHeader("Authorization", sas_token);

  int httpResponseCode = httpClient.POST(jsonBuffer);
  if (httpResponseCode > 0) {
    // Print HTTP response code
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    // Print HTTP response body
    String responseBody = httpClient.getString();
    Serial.println(responseBody);
  } else {
    Serial.print("HTTP Error Code: ");
    Serial.println(httpResponseCode);
    Serial.print("HTTP Error Message: ");
    Serial.println(httpClient.errorToString(httpResponseCode).c_str());
    Serial.println("Failed to send data");
  }

  delay(1000);
}