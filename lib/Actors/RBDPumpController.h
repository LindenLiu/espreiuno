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
#define ALL_DIMMERS 1
#include <stdlib.h>
#include "PumpController.h"
#include <RBDdimmer.h>


class RBDPumpController : public PumpController
{
private:
  static uint8_t range;
  static uint8_t zcPin;
  static uint8_t outputPin;
  static bool isPumpOn;
  static unsigned int value;
  static unsigned int counter;
  static void onZeroCross();
  dimmerLamp* dimmer;
public:
  RBDPumpController(uint8_t pin, uint8_t zc_pin, uint8_t range);
  ~RBDPumpController();
  void begin() override;
  void setDesiredPressure(float bar) override;
  void setOn(bool isOn) override;
  bool isOn() override; 
};

uint8_t RBDPumpController::range = 127;
uint8_t RBDPumpController::zcPin = 255;
uint8_t RBDPumpController::outputPin = 255;
unsigned int RBDPumpController::value = 0;
unsigned int RBDPumpController::counter = 0;
bool RBDPumpController::isPumpOn = false;


RBDPumpController::RBDPumpController(uint8_t pin, uint8_t theZcPin, uint8_t theRange)
{
  dimmer = new dimmerLamp(pin);
}

RBDPumpController::~RBDPumpController()
{
  delete dimmer;
}

void RBDPumpController::begin() 
{
  // pinMode(outputPin, OUTPUT);
  // pinMode(zcPin, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(zcPin), onZeroCross, FALLING);
  // RBDPumpController::value = RBDPumpController::range;
  // RBDPumpController::counter = 0;
  dimmer->begin(NORMAL_MODE, ON);
}

void RBDPumpController::setDesiredPressure(float bar) 
{
   dimmer->setPower((bar -1)*2.75+51);
  // RBDPumpController::value = (bar/9)*RBDPumpController::range;
}


void RBDPumpController::setOn(bool isOn) 
{
  RBDPumpController::isPumpOn = isOn;
}

bool RBDPumpController::isOn() 
{
  return dimmer->getState() == ON;
}

void RBDPumpController::onZeroCross()
{
  // RBDPumpController::counter += RBDPumpController::value;
  // unsigned int a = RBDPumpController::counter / RBDPumpController::range;
  // RBDPumpController::counter = RBDPumpController::counter % RBDPumpController::range;
  // digitalWrite(RBDPumpController::outputPin, RBDPumpController::isPumpOn && a > 0);
}