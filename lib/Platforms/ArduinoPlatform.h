#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "Platform.h"

class ArduinoPlatform : public Platform
{
private:
  /* data */
public:
  ArduinoPlatform(/* args */);
  ~ArduinoPlatform();
  void setPinMode(uint8_t pin, uint8_t mode) override;
  void analogWrite(uint8_t pin, int value) override;
  int analogRead(uint8_t pin) override;
  void analogReference(uint8_t mode) override;
  void digitalWrite(uint8_t pin, uint8_t val) override;
  int digitalRead(uint8_t pin) override;
  void turnOffPWM(uint8_t timer) override;
};

ArduinoPlatform::ArduinoPlatform(/* args */)
{
}

ArduinoPlatform::~ArduinoPlatform()
{
}

void ArduinoPlatform::setPinMode(uint8_t pin, uint8_t mode)
{
  pinMode(pin, mode);
}

void ArduinoPlatform::analogWrite(uint8_t pin, int value)
{
  analogWrite(pin, value);
}

int ArduinoPlatform::analogRead(uint8_t pin)
{
  return analogRead(pin);
}

void ArduinoPlatform::analogReference(uint8_t mode)
{
  analogReference(mode);
}

void ArduinoPlatform::digitalWrite(uint8_t pin, uint8_t val)
{
  digitalWrite(pin, val);
}

int ArduinoPlatform::digitalRead(uint8_t pin)
{
  return digitalRead(pin);
}

void ArduinoPlatform::turnOffPWM(uint8_t timer)
{
  turnOffPWM(timer);
}