#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

class wifi_connection {
    private:
        String ssid;
        String password ;

    public:
        wifi_connection(String ssid, String password) {
            this->ssid = ssid;
            this->password = password;

            WiFiServer server(80);
        }

        void setSSID(String ssid) {
            this->ssid = ssid;
        }

        void setPassword(String password) {
            this->password = password;
        }

        String getIP() const {
            return WiFi.localIP().toString();
        }

        bool getStatus() const {
            return WiFi.status() == WL_CONNECTED;
        }

/**
 * @brief Connect to the wifi
 * 
 * @param timeout_ms time to wait for the wifi to connect
 * @return bool time to wait for the wifi to connect 
 */
        bool connect(int timeout_ms) {
            unsigned long start = millis();

            WiFiServer server(80);

            WiFi.begin(ssid, password);

            while (WiFi.status() != WL_CONNECTED && millis() - start > timeout_ms) {
                delay(500);
                Serial.print(".");
            }

            return WiFi.status() == WL_CONNECTED;
        }        
};