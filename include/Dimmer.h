#pragma once

class Dimmer
{
private:
  /* data */
public:
  virtual void begin(){};
  virtual void setPower(int power){};
  virtual int getPower() 
  {
    return 0;
  };
  virtual void setOn(bool isOn){};
  virtual bool isOn() 
  {
    return false;
  }; 
};

