/**
 * MIT License
 *
 * Copyright (c) 2022 Linden Liu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

#define TEMP_SENSOR_READ_INTERVAL 30 // Minimum time between tow reads. Unit milliseconds
#include <Adafruit_MAX31865.h>
#include <SPI.h>
#include "TemperatureSensor.h"

typedef enum PTSensorType {
  PT_100 = 100,
  PT_1000 = 1000
} PTSensor_Type_t;

class RTDTemperatureSensor : public TemperatureSensor
{
private:
  Adafruit_MAX31865 sensor;
  PTSensorType ptType;
  float refResistor;
  max31865_numwires_t wireNum;

public:
  RTDTemperatureSensor(uint8_t csPin, PTSensorType ptType, float refResistor, max31865_numwires_t wireNum);
  ~RTDTemperatureSensor();
  void begin() override;
  float readCelsius() override;
  void clearFaultCode() override;
  uint8_t sensorFaultCode() override;
};

RTDTemperatureSensor::RTDTemperatureSensor(uint8_t csPin, PTSensorType ptType, float refResistor, max31865_numwires_t wireNum): 
  ptType(ptType), refResistor(refResistor), wireNum(wireNum), sensor(csPin)
{
 
}

RTDTemperatureSensor::~RTDTemperatureSensor()
{
  //delete sensor;
}

void RTDTemperatureSensor::begin() 
{
  sensor.begin(wireNum);
}

float RTDTemperatureSensor::readCelsius() 
{
  return sensor.temperature(100, 430);
}

void RTDTemperatureSensor::clearFaultCode() {
  sensor.clearFault();
}

uint8_t RTDTemperatureSensor::sensorFaultCode() {
  return sensor.readFault();
}