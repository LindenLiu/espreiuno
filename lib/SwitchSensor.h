#pragma once

class SwitchSensor
{
public:
  virtual void begin() {};
  virtual bool isOn()
  {
    return false;
  };
};
