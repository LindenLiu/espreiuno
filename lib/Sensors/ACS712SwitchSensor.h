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
#pragma once

#include <ACS712.h>
#include "SwitchSensor.h"

class ACS712SwitchSensor : public SwitchSensor
{
private:
  ACS712 *sensor;
  uint16_t offCurrentMA; // mA
  uint8_t freq;
public:
  ACS712SwitchSensor(uint8_t pin, ACS712_type type, uint16_t offCurrentMA, uint8_t freq);
  ~ACS712SwitchSensor();
  void begin() override;
  bool isOn() override;
};

ACS712SwitchSensor::ACS712SwitchSensor(uint8_t pin, ACS712_type type, uint16_t offCurrentMA, uint8_t freq) : offCurrentMA(offCurrentMA), freq(freq)
{
  sensor = new ACS712(type, pin);
}

ACS712SwitchSensor::~ACS712SwitchSensor()
{
  delete sensor;
}

void ACS712SwitchSensor::begin() 
{
  sensor->calibrate();
}

bool ACS712SwitchSensor::isOn()
{
  return sensor->getCurrentAC(freq) >= offCurrentMA;
}
