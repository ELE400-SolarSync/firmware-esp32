#include <Arduino.h>
#include <unity.h>
#include "../lib/api/src/api.hpp"
#include "../lib/wifi/src/wifi.hpp"

const String api_url = "https://randomuser.me/api/";
const String wifi_ssid = "Siri-al_killer";
const String wifi_password = "15112001";

wifi_connection wifi(wifi_ssid, wifi_password);

api_lib api;

void setUp(void)
{
  // set stuff up here
}

void tearDown(void)
{
  // clean stuff up here
}

void test_api_call(void)
{
  TEST_ASSERT_EQUAL(200, api.getCodeTest(api_url));
}

void test_wifi_connection(void)
{
  delay(1000);
  TEST_ASSERT_TRUE(wifi.getStatus());
}

void setup()
{
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  wifi.connect(10000);

  UNITY_BEGIN(); // IMPORTANT LINE!
  
}

void loop()
{
  RUN_TEST(test_wifi_connection);
  RUN_TEST(test_api_call);
  UNITY_END(); // stop unit testing
}