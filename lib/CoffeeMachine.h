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

#include <stdint.h>
#include <CRC32.h>
#include <EEPROM.h>

#include "PumpController.h"
#include "SwitchSensor.h"
#include "TemperatureSensor.h"
#include "BoilerController.h"
#include "Controllers/PIDBoilerController.h"
#include "Controllers/SteamBoilerController.h"
#include "Gui.h"
#include "PressureTransducer.h"

#define NO_ERROR 0

typedef struct CoffeeMachineState
{
  int boilerPwm = 0;
  double currentTemp = 0;
  uint8_t tempSensorErrorCode = NO_ERROR;
  float pressure = 0;
  SwitchState_t brewSwitchState;
  SwitchState_t steamSwitchState;
  unsigned long lastEvaluation = 0;
} CoffeeMachineState_t;


class CoffeeMachine
{
private:
  PumpController &pumpControl;
  SwitchSensor &steamSwitch;
  SwitchSensor &brewSwitch;
  TemperatureSensor &boilerTemp;
  Gui &gui;
  PressureTransducer &pressureSensor;
  PIDBoilerController *pidController;
  SteamBoilerController *steamController;
  
  uint8_t boilerSsrPin;
  // TODO: Make sure read and write automic
  volatile CoffeeMachineState_t state;
  CoffeeMachineConfig_t config;
  volatile uint16_t sampleInterval = 100;
  // Read temperature from sensor, if there is error, put it to tempSensorErrorCode
  void readTemperature();
  // Update gui
  void updateGui();
  // Control the temperature of boiler.
  void controlBoiler();
  // Check the brew control();
  void controlBrew();
  // 
  void descale();
  // 
  void backflush();

  uint32_t calculateSum(CoffeeMachineConfig_t &configData);

  void readConfig();
  void saveConfig();
public:
  CoffeeMachine(PumpController &pumpControl,
                SwitchSensor &steamSwitch,
                SwitchSensor &brewSwitch,
                TemperatureSensor &boilerTemp,
                Gui &guiImpl,
                PressureTransducer &pressureSensorImpl,
                uint8_t boilerSsrPin);
  ~CoffeeMachine();
  void begin();
  void loop();
  void onSaveTriggered();
};

CoffeeMachine::CoffeeMachine(PumpController &pumpControl,
                SwitchSensor &steamSwitch,
                SwitchSensor &brewSwitch,
                TemperatureSensor &boilerTemp,
                Gui &guiImpl,
                PressureTransducer &pressureSensorImpl,
                uint8_t boilerSsrPin)
    : pumpControl(pumpControl),
      steamSwitch(steamSwitch),
      brewSwitch(brewSwitch),
      boilerTemp(boilerTemp),
      gui(guiImpl),
      pressureSensor(pressureSensorImpl),
      boilerSsrPin(boilerSsrPin)
{
  
  pidController = new PIDBoilerController(config.pidParams, config.targetBrewTemp, 0);
  steamController = new SteamBoilerController(1);
  
}

CoffeeMachine::~CoffeeMachine()
{
  delete pidController;
  delete steamController;
}

void CoffeeMachine::begin()
{
  Serial.begin(115200);
  this->readConfig();
  pinMode(boilerSsrPin, OUTPUT);
  this->gui.begin();
  this->pumpControl.begin();
  this->steamSwitch.begin();
  this->brewSwitch.begin();
  this->boilerTemp.begin();
  this->pidController->begin();
  this->steamController->begin();
  this->pressureSensor.begin();
  updateGui();
  gui.setTargetTemperature(config.targetBrewTemp);
  gui.setPidParam(config.pidParams);
}

void CoffeeMachine::loop()
{
  double now = millis();
  gui.loop();
  if (now - state.lastEvaluation > sampleInterval)
  {
    readTemperature();
    // state.pressure = pressureSensor.getPressure();
    state.pressure = 9;
    controlBoiler();
    controlBrew();
    analogWrite(boilerSsrPin, state.boilerPwm);
    updateGui();
    state.lastEvaluation = now;
  }
  
}

void CoffeeMachine::onSaveTriggered()
{

  config.pidParams = gui.getPidParam();
  config.targetBrewTemp = gui.getTargetTemperature();
  config.targetSteamTemp = gui.getTargetSteamTemperature();
  Serial.println("Save to eeprom");
  this->saveConfig();
}


void CoffeeMachine::updateGui() 
{
  gui.setBoilerState(state.boilerPwm);
  gui.setTemperature(state.currentTemp);
  gui.setPressure(state.pressure);
  gui.setBrewSwitchState(state.brewSwitchState); 
}

void CoffeeMachine::readTemperature()
{
  state.currentTemp = boilerTemp.readCelsius();
  state.tempSensorErrorCode = boilerTemp.sensorFaultCode();
  if (state.tempSensorErrorCode != NO_ERROR) 
  {
    boilerTemp.clearFaultCode();
  }
}

void CoffeeMachine::controlBoiler()
{
  state.steamSwitchState = steamSwitch.isOn() ? SWITCH_ON : SWITCH_OFF;
  int boilerOutput = 0;
  if (state.tempSensorErrorCode == NO_ERROR)
  {
    if (state.steamSwitchState == SWITCH_OFF)
    {
      boilerOutput = pidController->boilerPwmValue(config.targetBrewTemp, state.currentTemp);
    }
    else
    {
      boilerOutput = steamController->boilerPwmValue(config.targetSteamTemp, state.currentTemp);
    }
  }
  state.boilerPwm = boilerOutput;
}

void CoffeeMachine::controlBrew()
{
  
  if (brewSwitch.isOn() && state.brewSwitchState != SWITCH_ON)
  {
    // If brewcontroller is null create
    // continue brewing process (preInfusing, auto pressure profiling etc)
    state.brewSwitchState = SWITCH_ON;
  }
  else if (!brewSwitch.isOn()) 
  {
    // delete last brew controller;
    state.brewSwitchState = SWITCH_OFF;
  }
}

void CoffeeMachine::readConfig() 
{
  CoffeeMachineConfig_t theConfig;
  EEPROM.get(0, theConfig);
  uint32_t crc = calculateSum(theConfig);
  if (crc == theConfig.crc) {
    Serial.println("Use EEPROM config");
    this->config = theConfig;
  }
}

void CoffeeMachine::saveConfig()
{
  this->config.crc = calculateSum(this->config);
  EEPROM.put(0, this->config);
}

uint32_t CoffeeMachine::calculateSum(CoffeeMachineConfig_t &configData) 
{
  CRC32 crc;
  crc.setPolynome(0x04C11DB7);
  crc.add((uint8_t*)&configData, sizeof(configData) - sizeof(configData.crc));
  return crc.getCRC();
}
