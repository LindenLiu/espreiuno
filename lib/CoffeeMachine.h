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
#include <stdint.h>
#include <ArduinoInterface.h>

#include "Dimmer.h"
#include "SwitchSensor.h"
#include "TemperatureSensor.h"
#include "BoilerController.h"
#include "Controllers/PIDBoilerController.h"
#include "Controllers/SteamBoilerController.h"

#define NO_ERROR 0

typedef struct CoffeeMachineState
{
  bool isBoilerOn = false;
  double currentTemp = 0;
  uint8_t tempSensorErrorCode = NO_ERROR;
  unsigned long lastEvaluation = 0;
} CoffeeMachineState;


class CoffeeMachine
{
private:
  Dimmer &pumpControl;
  SwitchSensor &steamSwitch;
  SwitchSensor &brewSwitch;
  TemperatureSensor &boilerTemp;
  PIDBoilerController *pidController;
  SteamBoilerController *steamController;
  uint8_t boilerSsrPin;
  CoffeeMachineState state;
  uint16_t sampleInterval = 100;
  int i;
  // Read temperature from sensor, if there is error, put it to tempSensorErrorCode
  void updateTemperature();
  // Control the temperature of boiler.
  void controlBoiler();
  // Check the brew control();
  void controlBrew();
  // 
  void descale();
  // 
  void backflush();
public:
  CoffeeMachine(Dimmer &pumpControl,
                SwitchSensor &steamSwitch,
                SwitchSensor &brewSwitch,
                TemperatureSensor &boilerTemp,
                uint8_t boilerSsrPin);
  ~CoffeeMachine();
  void begin();
  void run();
};

CoffeeMachine::CoffeeMachine(Dimmer &pumpControl,
                SwitchSensor &steamSwitch,
                SwitchSensor &brewSwitch,
                TemperatureSensor &boilerTemp,
                uint8_t boilerSsrPin)
    : pumpControl(pumpControl),
      steamSwitch(steamSwitch),
      brewSwitch(brewSwitch),
      boilerTemp(boilerTemp),
      boilerSsrPin(boilerSsrPin)
{
  // read saved parameters
  PIDParams_t pidParams;
  pidController = new PIDBoilerController(pidParams, 90, 20);
  steamController = new SteamBoilerController(1);
  i = 0;
}

CoffeeMachine::~CoffeeMachine()
{
  
}

void CoffeeMachine::begin()
{
  pinMode(6, OUTPUT);
  this->pumpControl.begin();
  this->steamSwitch.begin();
  this->brewSwitch.begin();
  this->boilerTemp.begin();
  this->pidController->begin();
  this->steamController->begin();
}

void CoffeeMachine::run()
{
  double now = millis();

  if (now - state.lastEvaluation > sampleInterval)
  {
    updateTemperature();
    controlBoiler();
    controlBrew();
    digitalWrite(6, state.isBoilerOn ? HIGH: LOW);
    state.lastEvaluation = millis();
  }
}

void CoffeeMachine::updateTemperature()
{
  
  state.currentTemp = boilerTemp.readCelsius() + i;
  state.tempSensorErrorCode = boilerTemp.sensorFaultCode();
  Serial.print("Temperature: ");
  Serial.println(state.currentTemp);
  Serial.print("Fault: ");
  Serial.println(state.tempSensorErrorCode);
	delay(1000);
  boilerTemp.clearFaultCode();
}

void CoffeeMachine::controlBoiler()
{
  bool shouldBoilerOn = false;
  // If the temperature sensor returns error, put boiler down.
  // Serial.print("Boiler temp error :");
  // Serial.print(state.tempSensorErrorCode);
  // Serial.print(", steam on: ");
  // Serial.println(steamSwitch.isOn());
  if (state.tempSensorErrorCode == NO_ERROR)
  {
    //if (!steamSwitch.isOn())
    {
      shouldBoilerOn = pidController->shouldBoilerOn(90, state.currentTemp);
      Serial.print("Boiler On:");
      Serial.println(shouldBoilerOn);
    }
    //else
    //{
    //  shouldBoilerOn = steamController->shouldBoilerOn(100, state.currentTemp);
    //}
  }
  else
  {
    // Output Error to Screen
  }
  // Put boiler on or off for this cycle.
  arduino.writeDigital(boilerSsrPin, shouldBoilerOn);
  state.isBoilerOn = shouldBoilerOn;
  i += shouldBoilerOn;
}

void CoffeeMachine::controlBrew()
{
  if (brewSwitch.isOn())
  {
    // If brewcontroller is null create
    // continue brewing process (preInfusing, auto pressure profiling etc)
  }
  else // if (brewController != null)
  {
    // delete last brew controller;
  }
}