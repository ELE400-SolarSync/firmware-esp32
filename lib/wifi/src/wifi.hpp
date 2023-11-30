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

        bool isConnected() const {
            return WiFi.isConnected();
        }

        /**
         * @brief Connect to the wifi
         * 
         * @param timeout_ms time to wait for the wifi to connect
         * @return bool time to wait for the wifi to connect 
         */
        bool connect(int max_attempts) {
            int attempts = 0;

            IPAddress staticIP(192, 168, 1, 100);
            IPAddress gateway(192, 168, 1, 1);
            IPAddress subnet(255, 255, 255, 0); 

            WiFiServer server(80);

            Serial.print("Connecting to WiFi ");

            while (WiFi.status() != WL_CONNECTED && attempts < max_attempts) {
                WiFi.begin(ssid, password);
                delay(1000);
                Serial.print(".");
                attempts ++;
            }
            Serial.println();

            return WiFi.status() == WL_CONNECTED;
        }

        bool disconnect() {
            return WiFi.disconnect(true);
        }

        bool setModeOff() {
            return WiFi.mode(WIFI_OFF);
        }
};