#include <WiFi.h>
#include <HTTPClient.h>

class API {
    private:
        char* host = "";

    public:
        void setHost(char* host) {
            this->host = host;
        }

        void get(char* path) {
            HTTPClient http;
            http.begin(strcat(host, path));
            int httpCode = http.GET();
            if (httpCode > 0) {
                String payload = http.getString();
                Serial.println(httpCode);
                Serial.println(payload);
            }
            http.end();
        }
};