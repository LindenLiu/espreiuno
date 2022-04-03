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

#include <QuickPID.h>
#include <Wire.h>
#include "BoilerController.h"

class PIDBoilerController : public BoilerController<PIDParams_t>
{
private:
  float setPoint;
  float input;
  float output;
  uint16_t sampleTimeMs;
  QuickPID *pid;
public:
  PIDBoilerController(const PIDParams_t &params, double targetTemperature, double initTemp);
  ~PIDBoilerController();
  void begin() override;
  bool changeControlParams(const PIDParams_t &params) override;
  int boilerPwmValue(double target, double currentTemperature) override;
};

PIDBoilerController::PIDBoilerController(const PIDParams_t &params, double targetTemperature, double initTemp) 
  :setPoint(targetTemperature), input(initTemp), sampleTimeMs(params.sampleTime)
{
  this->setPoint = targetTemperature;
  this->input = initTemp;
  this->pid = new QuickPID(&input, &output, &this->setPoint, params.kP, params.kI, params.kD, QuickPID::Action::direct);
  this->pid->SetMode(QuickPID::Control::automatic);
  this->pid->SetSampleTimeUs(params.sampleTime*1000);
  this->pid->SetOutputLimits(0, 1);
}

PIDBoilerController::~PIDBoilerController()
{
  delete pid;
}

void PIDBoilerController::begin()
{
 
}

int PIDBoilerController::boilerPwmValue(double target, double currentTemperature) 
{
  if (abs(currentTemperature - target) <= 0.01 ) {
    return 0;
  }
  
  this->setPoint = target;
  this->input = currentTemperature;
  this->pid->Compute(); 

  return this->output * 255;
}

bool PIDBoilerController::changeControlParams(const PIDParams_t &newParams) {
  if (sampleTimeMs > MIN_SAMPLE_TIME) {
    this->pid->SetTunings(newParams.kP, newParams.kI, newParams.kD);
    sampleTimeMs = newParams.sampleTime;
    this->pid->SetSampleTimeUs(newParams.sampleTime*1000);
    return true;
  }

  return false;
}





