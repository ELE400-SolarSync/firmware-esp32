#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

class wifi_connection {
    private:
        String ssid;
        String password ;

    public:
        /**
         * @brief Construct a new wifi connection object
         * 
         * @param ssid 
         * @param password 
         */
        wifi_connection(String ssid, String password) {
            this->ssid = ssid;
            this->password = password;

            WiFiServer server(80);
        }

        /**
         * @brief Set the SSID parameter
         * 
         * @param log_path 
         */
        void setSSID(String ssid) {
            this->ssid = ssid;
        }

        /**
         * @brief Set the Password parameter
         * 
         * @param password 
         */
        void setPassword(String password) {
            this->password = password;
        }

        /**
         * @brief Get the IP
         * 
         * @return String 
         */
        String getIP() const {
            return WiFi.localIP().toString();
        }

        /**
         * @brief Get the wifi status
         * 
         * @return true 
         * @return false 
         */
        bool getStatus() const {
            return WiFi.status() == WL_CONNECTED;
        }

        /**
         * @brief Check if the wifi is connected
         * 
         * @return true 
         * @return false 
         */
        bool isConnected() const {
            return WiFi.isConnected();
        }

        /**
         * @brief Connect to the wifi in less than max_attempts
         * 
         * @param max_attempts number of attempts for the wifi to connect
         * @return bool wifi connected or not
         */
        bool connect(int max_attempts) {
            int attempts = 0;

            // IPAddress staticIP(192, 168, 1, 100);
            // IPAddress gateway(192, 168, 1, 1);
            // IPAddress subnet(255, 255, 255, 0); 

            // WiFiServer server(80);

            Serial.println("Connecting to " + ssid);
            while (WiFi.status() != WL_CONNECTED && attempts < max_attempts) {
                WiFi.begin(ssid, password);
                delay(1000);
                Serial.print(".");
                attempts ++;
            }
            Serial.println();

            return WiFi.status() == WL_CONNECTED;
        }

        /**
         * @brief Disconnect from the wifi
         * 
         * @return true 
         * @return false 
         */
        bool disconnect() {
            return WiFi.disconnect(true);
        }

        /**
         * @brief Disable the wifi
         * 
         * @param mode 
         * @return true 
         * @return false 
         */
        bool setModeOff() {
            return WiFi.mode(WIFI_OFF);
        }
};