#pragma once 

class TemperatureSensor
{ 
  public:
    virtual void begin();
    virtual float readCelsius();
    virtual float readFahrenheit();
    virtual void clearFaultCode();
    virtual uint8_t sensorFaultCode();
};
