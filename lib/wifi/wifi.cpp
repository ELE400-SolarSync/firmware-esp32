#include <WiFi.h>
#include <HTTPClient.h>

class wifi_connection {
    public:
        void setSSID(const char* ssid) {
            this->ssid = ssid;
        }

        void setPassword(const char* password) {
            this->password = password;
        }

        void connect() {
            WiFi.begin(ssid, password);
            while (WiFi.status() != WL_CONNECTED) {
                delay(500);
                Serial.print(".");
            }
            Serial.println("");
            Serial.println("WiFi connected");
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());
        }

    private:
        const char* ssid = "ssid";
        const char* password = "password";
};