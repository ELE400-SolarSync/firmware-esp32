#include <Arduino.h>
#include <unity.h>
#include <SPI.h>
#include "../lib/sd/src/sdcustom.hpp"

SDCustom sd(16);

void test_sd_insert(void)
{
  TEST_ASSERT_EQUAL_MESSAGE(true, sd.isSDInserted(), "No SD Detected. Please insert one.");
}

void test_sd_writing(void)
{
  if (!sd.fileExists("ESP32.txt")) {
    sd.createFile("ESP32.txt");
  }

  bool result = sd.fileExists("ESP32.txt");

  sd.deleteFile("ESP32.txt");

  TEST_ASSERT_TRUE(result);
}

void test_sd_reading(void)
{
  if(sd.fileExists("ESP32.txt")) {
    sd.deleteFile("ESP32.txt");
  }
  sd.writeFile("ESP32.txt", "Hello World!");
  String reading = sd.readFile("ESP32.txt");
  // sd.deleteFile("ESP32.txt");
  TEST_ASSERT_EQUAL_STRING("Hello World!\r\n", reading.c_str());
  
}

void setup()
{
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  sd.begin();

  UNITY_BEGIN(); // IMPORTANT LINE!
  
}

void loop()
{
  RUN_TEST(test_sd_insert);
  RUN_TEST(test_sd_writing);
  RUN_TEST(test_sd_reading);
  UNITY_END(); // stop unit testing
}