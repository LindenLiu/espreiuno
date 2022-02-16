#pragma once

#include "../SwitchSensor.h"

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
  pinMode(pin, INPUT);
}

bool SimpleSwitch::isOn() 
{
  return analogRead(this->pin) == 0;
}