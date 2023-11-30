#include <Adafruit_Sensor.h>
#include <DHT.h>

class DHTSensor {
    private:
        DHT dht_sen;
        int pin;

    public:
        /**
         * @brief Construct a new DHTSensor object
         * 
         * @param pin DHT11 pin
         */
        DHTSensor(int pin) : dht_sen(pin, DHT11), pin(pin) {
            dht_sen.begin();
        }

        /**
         * @brief Get the Humidity object
         * 
         * @return float Humidity value
         */
        float getHumidity() {
            return dht_sen.readHumidity();
        }

        /**
         * @brief Get the Temperature object
         * 
         * @return float Temperature value
         */
        float getTemperature() {
            return dht_sen.readTemperature();
        }

        /**
         * @brief Get the Values object
         * 
         * @return float* Humidity and Temperature values
         */
        float *getValues() {
            float *values = new float[2];
            values[0] = getHumidity();
            values[1] = getTemperature();
            return values;
        }

        /**
         * @brief Check if the values are correct
         * 
         * @param values Array of values
         * @return bool True if values are correct
         */
        bool isCorrect_values(float *values) {
            return !(isnan(values[0]) || isnan(values[1]));
        }

        /**
         * @brief Check if the sensor is ready
         * 
         * @return bool True if the sensor is ready
         */
        bool isReady() {
            return getHumidity() && getTemperature();
        }
};