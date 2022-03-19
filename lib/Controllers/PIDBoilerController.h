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

#include <PID_v1.h>
#include <Wire.h>
#include "BoilerController.h"


class PIDBoilerController : public BoilerController<PIDParams_t>
{
private:
  double setPoint;
  double input;
  double output;
  uint16_t sampleTimeMs;
  PID *pid;
public:
  PIDBoilerController(const PIDParams_t &params, double targetTemperature, double initTemp);
  ~PIDBoilerController();
  void begin() override;
  bool changeControlParams(const PIDParams_t &params) override;
  bool shouldBoilerOn(const double &target, const double &currentTemperature) override;
};

PIDBoilerController::PIDBoilerController(const PIDParams_t &params, double targetTemperature, double initTemp) 
  :setPoint(targetTemperature), input(initTemp), sampleTimeMs(params.sampleTime)
{
  this->setPoint = targetTemperature;
  this->input = initTemp;
  this->pid = new PID(&input, &output, &this->setPoint, params.kP, params.kI, params.kD, DIRECT);
  this->pid->SetMode(AUTOMATIC);
  this->pid->SetSampleTime(params.sampleTime);
  this->pid->SetOutputLimits(0, 255);
}

PIDBoilerController::~PIDBoilerController()
{
  delete pid;
}

void PIDBoilerController::begin()
{
 
}

bool PIDBoilerController::shouldBoilerOn(const double &target, const double &currentTemperature) 
{
  this->setPoint = target;
  this->input = currentTemperature;
  bool c = this->pid->Compute(); 
  Serial.print("PID:");
  Serial.print(c);
  Serial.print(" output: ");
  Serial.println(this->output);
  boolean shouldOn = this->output > this->sampleTimeMs;
  return shouldOn;
}

bool PIDBoilerController::changeControlParams(const PIDParams_t &newParams) {
  if (sampleTimeMs > MIN_SAMPLE_TIME) {
    this->pid->SetTunings(newParams.kP, newParams.kI, newParams.kD);
    sampleTimeMs = newParams.sampleTime;
    this->pid->SetSampleTime(newParams.sampleTime);
    return true;
  }

  return false;
}


