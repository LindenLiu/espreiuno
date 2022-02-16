#pragma once

#include <RBDdimmer.h>
#include "../Dimmer.h"

class RBDDimmerActor : public Dimmer
{
private:
  dimmerLamp *dimmer;
public:
  RBDDimmerActor(uint8_t pin);
  ~RBDDimmerActor();
  void begin() override;
  void setPower(int power) override;
  int getPower() override;
  void setOn(bool isOn) override;
  bool isOn() override; 
};

RBDDimmerActor::RBDDimmerActor(uint8_t pin)
{
  dimmer = new dimmerLamp(pin);
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
