#pragma once

#include <PID_v1.h>
#include "../BoilerController.h"



class PIDBoilerController : public BoilerController<PIDParams_t>
{
private:
  double targetTemperature;
  double input;
  double output;
  uint16_t sampleTimeMs;
  PID *pid;
public:
  PIDBoilerController(const PIDParams_t &params, double targetTemperature, double initTemp);
  ~PIDBoilerController();
  void begin() override;
  bool changeControlParams(const PIDParams_t &params) override;
  const PIDParams_t& getControlParams() override;
  bool shouldBoilerOn(const double &target, const double &currentTemperature) override;
};

PIDBoilerController::PIDBoilerController(const PIDParams_t &params, double targetTemperature, double initTemp) 
  :targetTemperature(targetTemperature), input(initTemp), sampleTimeMs(params.sampleTime)
{
  this->targetTemperature = targetTemperature;
  this->input = initTemp;
  this->pid = new PID(&input, &output, &targetTemperature, params.kP, params.kI, params.kD, DIRECT);
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
  this->targetTemperature = target;
  this->input = currentTemperature;
  this->pid->Compute();
  return this->output > 100;
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

const PIDParams_t& PIDBoilerController::getControlParams() {
  PIDParams_t params;
  params.kD = this->pid->GetKd();
  params.kI = this->pid->GetKi();
  params.kP = this->pid->GetKp();
  params.sampleTime = sampleTimeMs;
  return params;
}


