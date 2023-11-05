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

        bool connect(int timeout_ms) {
            unsigned long start = millis();

            WiFiServer server(80);

            Serial.println("Connecting to WiFi ");
            WiFi.begin(ssid, password);

            while (WiFi.status() != WL_CONNECTED && millis() - start > timeout_ms) {
                delay(500);
                Serial.print(".");
            }

            if(WiFi.status() == WL_CONNECTED) {
                return true;
            }
            return false;
        }        
};