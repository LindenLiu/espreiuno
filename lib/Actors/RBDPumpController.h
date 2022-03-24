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

#include <RBDdimmer.h>
#include "PumpController.h"

class RBDPumpController : public PumpController
{
private:
  const uint8_t oneBarStartingPower = 51;
  const float oneBarStepPower = 2.5;
  dimmerLamp *dimmer;
public:
  RBDPumpController(uint8_t pin, uint8_t zc_pin);
  ~RBDPumpController();
  void begin() override;
  void setDesiredPressure(float bar) override;
  float getDesiredPressure() override;
  void setOn(bool isOn) override;
  bool isOn() override; 
};

RBDPumpController::RBDPumpController(uint8_t pin, uint8_t zc_pin)
{
  #if   defined(ARDUINO_ARCH_AVR)
	  dimmer = new dimmerLamp(pin);
  #elif defined(ARDUINO_ARCH_ESP32)
    dimmer = new dimmerLamp(pin, zc_pin);
  #endif
}

RBDPumpController::~RBDPumpController()
{
  delete dimmer;
}

void RBDPumpController::begin() 
{
  dimmer->begin(NORMAL_MODE, ON);
  dimmer->setPower(100);
}

void RBDPumpController::setDesiredPressure(float bar) 
{
  int power = round((bar-1) * this->oneBarStepPower + this->oneBarStartingPower);
  dimmer->setPower(power);
}

float RBDPumpController::getDesiredPressure() 
{
  return (dimmer->getPower() - this->oneBarStartingPower)/this->oneBarStepPower + 1;
}

void RBDPumpController::setOn(bool isOn) 
{
  dimmer->setState(isOn ? ON : OFF);
}

bool RBDPumpController::isOn() 
{
  return dimmer->getState() == ON;
}
