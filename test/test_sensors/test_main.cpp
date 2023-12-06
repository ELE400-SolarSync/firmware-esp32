#include <Arduino.h>
#include <unity.h>
#include <SPI.h>
#include "../lib/sensors/src/dht.hpp"
#include "../lib/sensors/src/voltage_current.hpp"

DHTSensor dht(10);
CurrentSensor current_solar(A2);
VoltageSensor voltage_solar(A0);

void test_dht_values(void)
{
  TEST_ASSERT_EQUAL_MESSAGE(dht.isCorrect_values(dht.getValues()));
}

void test_voltage_values(void)
{
    float values = voltage_solar.readVoltage();
    TEST_ASSERT_EQUAL_MESSAGE(true, values >= 0 && values <= 30);
}

void test_current_values(void)
{
    float values = current_solar.readCurrent();
    TEST_ASSERT_EQUAL_MESSAGE(true, values >= 0 && values <= 30);
}

void test_power(void)
{
    float v_values = voltage_solar.readVoltage();
    float c_values = current_solar.readCurrent();
    float power = current_solar.getPower(v_values, c_values);
    TEST_ASSERT_EQUAL_MESSAGE(true, power >= 0 && power <= 100);
}

void test_bat_level(void)
{
    float v_values = voltage_solar.readVoltage();
    float c_values = current_solar.readCurrent();
    float bat_level = current_solar.getBatLevel(v_values);
    TEST_ASSERT_EQUAL_MESSAGE(true, bat_level >= 0 && bat_level <= 100);
}

void setup()
{
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);


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