/**
 * MIT License
 *
 * Copyright (c) 2022 Linden Liu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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