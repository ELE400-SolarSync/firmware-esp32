#include <Arduino.h>

#define ADC_CORRECTION_FACTOR 1.15
#define ADC_BITS 4095.0
#define ADC_MAX_VOLTAGE 3.3
#define CURRENT_SENSOR_OFFSET 2.5
#define VOLTAGE_SENSOR_MULTIPLIER 5.0
#define CURRENT_SENSOR_MULTIPLIER 0.066

class VoltageSensor {
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
        return analogRead(pin) / ADC_CORRECTION_FACTOR;
    };

    /**
     * @brief Read the voltage from the pin
     * 
     * @return float 
     */
    float readVoltage() {
        return ADC_MAX_VOLTAGE * VOLTAGE_SENSOR_MULTIPLIER * read() / ADC_BITS;
    };

  private:
    int pin;
};

class CurrentSensor {
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
            return analogRead(pin) / ADC_CORRECTION_FACTOR;
        };
    
        /**
         * @brief Read the current from the pin
         * 
         * @return float 
         */
        float readCurrent(float param = CURRENT_SENSOR_OFFSET) {
            return ((ADC_MAX_VOLTAGE * read() / ADC_BITS) - param) / CURRENT_SENSOR_MULTIPLIER;
        };
    
    private:
        int pin;
};