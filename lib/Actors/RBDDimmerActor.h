#pragma once

#include <RBDdimmer.h>
#include "Dimmer.h"

class RBDDimmerActor : public Dimmer
{
private:
  dimmerLamp *dimmer;
public:
  RBDDimmerActor(uint8_t pin, uint8_t zc_pin);
  ~RBDDimmerActor();
  void begin() override;
  void setPower(int power) override;
  int getPower() override;
  void setOn(bool isOn) override;
  bool isOn() override; 
};

RBDDimmerActor::RBDDimmerActor(uint8_t pin, uint8_t zc_pin)
{
  #if   defined(ARDUINO_ARCH_AVR)
	  dimmer = new dimmerLamp(pin);
  #elif defined(ARDUINO_ARCH_ESP32)
    dimmer = new dimmerLamp(pin, zc_pin);
  #endif
}

RBDDimmerActor::~RBDDimmerActor()
{
  delete dimmer;
}

void RBDDimmerActor::begin() 
{
  dimmer->begin(NORMAL_MODE, OFF);
}

void RBDDimmerActor::setPower(int power) 
{
  dimmer->setPower(power);
}

int RBDDimmerActor::getPower() 
{
  return dimmer->getPower();
}

void RBDDimmerActor::setOn(bool isOn) 
{
  dimmer->setState(isOn ? ON : OFF);
}

bool RBDDimmerActor::isOn() 
{
  return dimmer->getState() == ON;
}
