#include <unity.h>

typedef unsigned char uint8_t;
#define INPUT 0
const uint8_t A0 = 0;
uint8_t switchValue = 0;

void pinMode(uint8_t pin, uint8_t mode) 
{
  TEST_ASSERT_EQUAL(A0, pin);
  TEST_ASSERT_EQUAL(INPUT, mode);
}

uint8_t analogRead(uint8_t pin) {
    TEST_ASSERT_EQUAL(A0, pin);
    return switchValue;
}

#include "SimpleSwitch.h"
SimpleSwitch simpleSwitch(A0);

void test_switch_is_on()
{
  switchValue = 0;
  TEST_ASSERT_TRUE(simpleSwitch.isOn());
}

void test_switch_is_off() 
{
  switchValue = 12;
  TEST_ASSERT_FALSE(simpleSwitch.isOn());
}

int main(int argc, char **argv) 
{
    UNITY_BEGIN();
    RUN_TEST(test_switch_is_on);
    RUN_TEST(test_switch_is_off);
    UNITY_END();
    return 0;
}