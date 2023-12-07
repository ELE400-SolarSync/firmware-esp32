#include <Arduino.h>

class VoltageSensor {
    private:
    int pin;
    const int adc_bits = 4095;
    const int adc_max_voltage = 3.3;
    const int voltage_sensor_multiplier = 5;
    const int adc_correction_factor = 1.15;

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
        return analogRead(pin) / adc_correction_factor;
    };

    /**
     * @brief Read the voltage from the pin
     * 
     * @return float 
     */
    float readVoltage() {
        return adc_max_voltage * voltage_sensor_multiplier * read() / adc_bits;
    };
};

class CurrentSensor {
    private:
        int pin;
        const float adc_bits = 4095.0;
        const float adc_max_voltage = 3.3;
        const float current_sensor_multiplier = 0.066;
        const float adc_correction_factor = 1.15;
        const float current_sensor_offset = 2.5;

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
            return analogRead(pin) / adc_correction_factor;
        };
    
        /**
         * @brief Read the current from the pin
         * 
         * @return float 
         */
        float readCurrent() {
            return ((adc_max_voltage * read() / adc_bits) - current_sensor_offset) / current_sensor_multiplier;
        };

        /**
         * @brief Get the Bat Level with the given current and the voltage
         * 
         * @param current 
         * @param voltage 
         * @return float 
         */
        float getBatLevel(float voltage) {
            return voltage;
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
        }
};