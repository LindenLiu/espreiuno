#pragma once

#include "Platform.h"
#include "SwitchSensor.h"

class SimpleSwitch : public SwitchSensor
{
private:
  uint8_t pin;
public:
  SimpleSwitch(uint8_t pin);
  ~SimpleSwitch();
  void begin() override;
  bool isOn() override;
};

SimpleSwitch::SimpleSwitch(uint8_t pin) : pin(pin)
{
}

SimpleSwitch::~SimpleSwitch()
{
}

void SimpleSwitch::begin() 
{
  platform->setPinMode(pin, INPUT);
}

bool SimpleSwitch::isOn() 
{
  return platform->analogRead(this->pin) == 0;
}