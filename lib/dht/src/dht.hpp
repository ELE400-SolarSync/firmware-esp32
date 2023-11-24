#include <Adafruit_Sensor.h>
#include <DHT.h>

class DHTSensor {
    private:
        DHT dht_sen;
        int pin;
        uint8_t type;

    public:
        DHTSensor(int pin, uint8_t type) : dht_sen(pin, type), pin(pin), type(type) {
            dht_sen.begin();
        }

        float getHumidity() {
            return dht_sen.readHumidity();
        }

        float getTemperature() {
            return dht_sen.readTemperature();
        }

        float *getValues() {
            float *values = new float[2];
            values[0] = getHumidity();
            values[1] = getTemperature();
            return values;
        }

        bool isCorrect_values(float *values) {
            return isnan(values[0]) && isnan(values[1]);
        }

        bool isReady() {
            return getHumidity() && getTemperature();
        }

        // void begin() {
        //     dht_sen = DHT(pin, type);
        //     dht_sen.begin();
        // }
};