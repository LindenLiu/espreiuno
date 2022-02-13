#pragma once

#define TEMP_SENSOR_READ_INTERVAL 30 // Minimum time between tow reads. Unit milliseconds
#include <Adafruit_MAX31865.h>
#include "../TemperatureSensor.h"

typedef enum PTSensorType {
  PT_100 = 100,
  PT_1000 = 1000
} PTSensor_Type_t;

class RTDTemperatureSensor : public TemperatureSensor
{
private:
  Adafruit_MAX31865 *sensor;
  PTSensorType ptType;
  float refResistor;
  max31865_numwires_t wireNum;

public:
  RTDTemperatureSensor(u8 csPin, PTSensorType ptType, float refResistor, max31865_numwires_t wireNum);
  ~RTDTemperatureSensor();
  void begin() override;
  float readCelsius() override;
  float readFahrenheit() override;
  void clearFaultCode() override;
  uint8_t sensorFaultCode() override;
};

RTDTemperatureSensor::RTDTemperatureSensor(u8 csPin, PTSensorType ptType, float refResistor, max31865_numwires_t wireNum): 
  ptType(ptType), refResistor(refResistor), wireNum(wireNum)
{
  sensor = new Adafruit_MAX31865(csPin);
}

RTDTemperatureSensor::~RTDTemperatureSensor()
{
  delete sensor;
}

void RTDTemperatureSensor::begin() 
{
  sensor->begin(wireNum);
}

float RTDTemperatureSensor::readCelsius() 
{
  return sensor->temperature(ptType, refResistor);
}

float RTDTemperatureSensor::readFahrenheit() {
  return readCelsius() * 9 / 5 + 32; 
}

void RTDTemperatureSensor::clearFaultCode() {
  sensor->clearFault();
}

uint8_t RTDTemperatureSensor::sensorFaultCode() {
  return sensor->readFault();
}