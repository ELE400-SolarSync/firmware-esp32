#include <Arduino.h>
#include <unity.h>
#include <SPI.h>
#include "../lib/sensors/src/dht.hpp"
#include "../lib/sensors/src/voltage_current.hpp"

DHTSensor dht(10);
CurrentSensor current_5v(A3);
VoltageSensor voltage_5v(A4);

const int relay_pin = 9;

void test_dht_values(void)
{
  TEST_ASSERT_TRUE(dht.isCorrect_values(dht.getValues()));
}

void test_voltage_values(void)
{
    float values = voltage_5v.readPinVoltage();
    TEST_ASSERT_TRUE(values >= 0 && values <= 30);
}

void test_current_values(void)
{
    float values = current_5v.readPinVoltage();
    TEST_ASSERT_TRUE(values >= 0 && values <= 100);
}

void test_power(void)
{
    float v_values = voltage_5v.readPinVoltage();
    float c_values = current_5v.readPinVoltage();
    float power = current_5v.getPower(v_values, c_values);
    TEST_ASSERT_TRUE(power >= 0 && power <= 100);
}

void test_bat_level(void)
{
    float v_values = voltage_5v.readPinVoltage();
    float c_values = current_5v.readPinVoltage();
    float bat_level = current_5v.getBatLevel(v_values);
    TEST_ASSERT_TRUE(bat_level >= 0 && bat_level <= 100);
}

void setup()
{
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, HIGH);

  UNITY_BEGIN(); // IMPORTANT LINE!
  
}

void loop()
{
  RUN_TEST(test_dht_values);
  RUN_TEST(test_voltage_values);
  RUN_TEST(test_current_values);
  RUN_TEST(test_power);
  RUN_TEST(test_bat_level);
  UNITY_END(); // stop unit testing
}