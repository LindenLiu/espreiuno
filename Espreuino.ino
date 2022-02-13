#include <Arduino.h>
#include "libraries/TemperatureSensor.h"
#include "libraries/SwitchSensor.h"
#include "libraries/Sensors/SimpleSwitch.h"
#include "libraries/Sensors/ASC712SwitchSensor.h"
#include "libraries/Actors/RBDDimmerActor.h"

#define PT100
#define FREQUENCY 60

#ifdef PT100
#define CSPIN A0
#include "libraries/Sensors/RTDTemperatureSensor.h"
TemperatureSensor *boilerTemp = new RTDTemperatureSensor(CSPIN, PT_100, 430,  MAX31865_2WIRE);
#endif

#define STEAM_PIN A7
SwitchSensor *steamSwitch = new SimpleSwitch(STEAM_PIN);

#define PUMP_SWITCH_PIN A0
#define PUMP_SWITCH_TYPE ACS712_20A
#define PUMP_OFF_CURRENT 400 //mA

SwitchSensor *pumpSwitch = new ACS712SwitchSensor(PUMP_SWITCH_PIN, PUMP_SWITCH_TYPE, PUMP_OFF_CURRENT, FREQUENCY);

void setup()
{
  steamSwitch->begin();
  // boilerTemp->begin();
}

void loop()
{
	
}
