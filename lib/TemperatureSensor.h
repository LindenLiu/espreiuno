#pragma once 


class TemperatureSensor
{ 
  public:
    virtual void begin() {};
    virtual float readCelsius()
    {
      return NAN;
    };
    virtual void clearFaultCode() {};
    virtual uint8_t sensorFaultCode() 
    {
      return 255;
    };
};
