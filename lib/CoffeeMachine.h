#pragma once

#include <PID_v1.h>

#include "Dimmer.h"
#include "SwitchSensor.h"
#include "TemperatureSensor.h"
#include "BoilerController.h"
#include "Controllers/PIDBoilerController.h"
#include "Controllers/SteamBoilerController.h"

#define NO_ERROR 0

class CoffeeMachine
{
private:
  Dimmer *pumpControl;
  SwitchSensor *steamSwitch;
  SwitchSensor *brewSwitch;
  TemperatureSensor *boilerTemp;
  PIDBoilerController *pidController;
  SteamBoilerController *steamController;
  double currentTemp;
  uint8_t tempSensorErrorCode = 0;
  long lastEvaluation = 0;
  uint16_t sampleInterval;
  // Read temperature from sensor, if there is error, put it to tempSensorErrorCode
  void readTemp();
  // Control the temperature of boiler.
  void controlBoiler();
  // Check the brew control();
  void controlBrew();
  // 
  void descale();
  // 
  void backflush();
public:
  CoffeeMachine(Dimmer *pumpControl,
                SwitchSensor *steamSwitch,
                SwitchSensor *brewSwitch,
                TemperatureSensor *boilerTemp);
  ~CoffeeMachine();
  void begin();
  void run();
};

CoffeeMachine::CoffeeMachine(Dimmer *pumpControl,
                             SwitchSensor *steamSwitch,
                             SwitchSensor *brewSwitch,
                             TemperatureSensor *boilerTemp) 
                             : pumpControl(pumpControl), 
                             steamSwitch(steamSwitch), 
                             brewSwitch(brewSwitch), 
                             boilerTemp(boilerTemp)
{
  // read saved parameters

}

CoffeeMachine::~CoffeeMachine()
{
  delete pumpControl;
  delete steamSwitch;
  delete brewSwitch;
  delete boilerTemp;
  delete pidController;
  delete steamController;
}

void CoffeeMachine::begin() {
  this->pumpControl->begin();
  this->steamSwitch->begin();
  this->brewSwitch->begin();
  this->boilerTemp->begin();
  this->pidController->begin();
  this->steamController->begin();
}

void CoffeeMachine::run() {
  double now = millis();
  bool shouldBoilerOn = false;
  if (now - lastEvaluation > sampleInterval) {
    readTemp();
    controlBoiler();
    controlBrew();
    
  }
  
}

void CoffeeMachine::readTemp() {
  currentTemp = boilerTemp->readCelsius();
  tempSensorErrorCode = boilerTemp->sensorFaultCode();
  boilerTemp->clearFaultCode();
}

void CoffeeMachine::controlBoiler() {
  bool shouldBoilerOn = false;
  if (tempSensorErrorCode != NO_ERROR) {
      if (!steamSwitch->isOn()) {
        shouldBoilerOn = pidController->shouldBoilerOn(90, currentTemp);
      } else {
        shouldBoilerOn = steamController->shouldBoilerOn(100, currentTemp);
      }
    } else 
    {
      // Output Error to Screen
    }
  // Put boiler on or off for this cycle.
}

void CoffeeMachine::controlBrew() 
{
  if (brewSwitch->isOn()) 
  {
    // If brewcontroller is null create 
    // continue brewing process (preInfusing, auto pressure profiling etc)

  } else // if (brewController != null) 
  {
    // delete last brew controller;
  }
}
