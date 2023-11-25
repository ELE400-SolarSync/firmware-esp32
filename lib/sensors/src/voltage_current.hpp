#include <Arduino.h>

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
     * @return float Value between 0 and 1023
     */
    float read() {
        return analogRead(pin);
    };

    /**
     * @brief Read the voltage from the pin
     * 
     * @return float 
     */
    float readVoltage() {
        return read() / 1023.0; // real math : (5.0 * read() / 1023.0) / 5.0 
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
         * @return float Value between 0 and 1023
         */
        float read() {
            return analogRead(pin);
        };
    
        /**
         * @brief Read the current from the pin
         * 
         * @return float 
         */
        float readCurrent(int param = 2.5) {
            return (read() * (5.0 / 1023.0) - param) / 0.066;
        };
    
    private:
        int pin;
};