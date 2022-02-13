#pragma once

#include <ACS712.h>
#include "../SwitchSensor.h"

class ACS712SwitchSensor : public SwitchSensor
{
private:
  ACS712 *sensor;
  uint16_t offCurrentMA; // mA
  uint8_t freq;
public:
  ACS712SwitchSensor(uint8_t pin, ACS712_type type, uint16_t offCurrentMA, uint8_t freq);
  ~ACS712SwitchSensor();
  void begin() override;
  bool isOn() override;
};

ACS712SwitchSensor::ACS712SwitchSensor(uint8_t pin, ACS712_type type, uint16_t offCurrentMA, uint8_t freq) : offCurrentMA(offCurrentMA), freq(freq)
{
  sensor = new ACS712(type, pin);
}

ACS712SwitchSensor::~ACS712SwitchSensor()
{
  delete sensor;
}

void ACS712SwitchSensor::begin() 
{
  sensor->calibrate();
}

bool ACS712SwitchSensor::isOn()
{
  return sensor->getCurrentAC(freq) >= offCurrentMA;
}
