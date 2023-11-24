#include <Arduino.h>
#include "../lib/sensors/src/voltage_current.hpp"

// const int battery_current_pin = A6;
// const int battery_voltage_pin = A7;

const int solar_current_pin = A2;
const int solar_voltage_pin = A3;

const int current_pin_5v = A4;
const int voltage_pin_5v = A5;

const int current_pin_12v = A0;
const int voltage_pin_12v = A1;

// CurrentSensor current_battery(battery_current_pin);
// VoltageSensor voltage_battery(battery_voltage_pin);

CurrentSensor current_solar(solar_current_pin);
VoltageSensor voltage_solar(solar_voltage_pin);

CurrentSensor current_5v(current_pin_5v);
VoltageSensor voltage_5v(voltage_pin_5v);

CurrentSensor current_12v(current_pin_12v);
VoltageSensor voltage_12v(voltage_pin_12v);

void setup() {
  Serial.begin(115200);

  // current_battery.setup();
  // voltage_battery.setup();

  current_solar.setup();
  voltage_solar.setup();

  current_5v.setup();
  voltage_5v.setup();

  current_12v.setup();
  voltage_12v.setup();
}

void loop() {
  // Serial.print("Battery: ");
  // Serial.print(current_battery.readCurrent());
  // Serial.print("A, ");
  // Serial.print(voltage_battery.readVoltage());
  // Serial.println("V");

  Serial.print("Solar: ");
  Serial.print(current_solar.readCurrent());
  Serial.print("A, ");
  Serial.print(voltage_solar.readVoltage());
  Serial.println("V");

  Serial.print("5V: ");
  Serial.print(current_5v.readCurrent());
  Serial.print("A, ");
  Serial.print(voltage_5v.readVoltage());
  Serial.println("V");

  Serial.print("12V: ");
  Serial.print(current_12v.readCurrent());
  Serial.print("A, ");
  Serial.print(voltage_12v.readVoltage());
  Serial.println("V");

  Serial.println();

  delay(1000);
}