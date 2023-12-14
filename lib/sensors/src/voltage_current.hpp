#include <Arduino.h>

class VoltageSensor {
    private:
    int pin;
    static constexpr float adc_bits = 4095.0f;
    static constexpr float adc_max_voltage = 3.3f;
    static constexpr float voltage_sensor_multiplier = 5.0f;

    public:
        /**
         * @brief Construct a new Voltage Sensor object
         * 
         * @param pin 
         */
        VoltageSensor(int pin) {
            this->pin = pin;
        };
        
        /**
         * @brief Setup the pin as input
         */
        void setup() {
            pinMode(pin, INPUT);
        };

        /**
         * @brief Read the raw value from the pin
         *
         * @return float Value between 0 and 4095
         */
        float read() {
            return analogRead(pin);
        };

        /**
         * @brief Read the voltage from the pin
         * 
         * @return float 
         */
        float readPinVoltage() {
            return 0.000698388508841322 * read() + 0.0213329188224827;
        };

        /**
         * @brief Read the voltage from the sensor
         * 
         * @return float 
         */
        float readVoltage() {
            return voltage_sensor_multiplier * readPinVoltage();
        };

        static float CorrectReading(float ref_ratio_5v, float voltage) {
            return voltage / ref_ratio_5v;
        };
};

class CurrentSensor {
    private:
        int pin;
        static constexpr float adc_bits = 4095.0;
        static constexpr float current_sensor_multiplier = 0.066;
        static constexpr float current_sensor_offset = 2.5;

    public:
        /**
         * @brief Construct a new Current Sensor object
         * 
         * @param pin 
         */
        CurrentSensor(int pin) {
            this->pin = pin;
        };
        
        /**
         * @brief Setup the pin as input
         */
        void setup() {
            pinMode(pin, INPUT);
        };
    
        /**
         * @brief Read the raw value from the pin
         * 
         * @return float Value between 0 and 4095
         */
        float read() {
            return analogRead(pin);
        };
    
        /**
         * @brief Read the voltage from the pin
         * 
         * @return float 
         */
        float readPinVoltage() {
            return 0.000698388508841322 * read() + 0.0213329188224827;
        };

        /**
         * @brief Read the current from the sensor
         * 
         * @return float 
         */
        float readCurrent() {
            return (readPinVoltage() - current_sensor_offset) / current_sensor_multiplier;
        };

        /**
         * @brief Get the Bat Level with the given current and the voltage
         * 
         * @param current 
         * @param voltage 
         * @return float 
         */
        float getBatLevel(float voltage) {
            return 5 * pow(10, -20) * pow(voltage, 19.263);
        }

        /**
         * @brief Get the Power with the given current and the voltage
         * 
         * @param current 
         * @param voltage 
         * @return float 
         */
        float getPower(float current, float voltage) {
            return current * voltage;
        };

        static float CorrectReading(float ref_ratio_5v, float current) {
            float recovered_pin_voltage = current * current_sensor_multiplier + current_sensor_offset;
            return (recovered_pin_voltage / ref_ratio_5v - current_sensor_offset) / current_sensor_multiplier;
        };
};