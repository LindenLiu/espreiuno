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
#include <EEPROM.h>
#include <Wire.h>
#include <Arduino.h>
#include "PlatformConfig.h"
#include "TemperatureSensor.h"
#include "SwitchSensor.h"
#include "../lib/Sensors/SimpleSwitch.h"
#include "../lib/Actors/RBDPumpController.h"
#include "../lib/CoffeeMachine.h"
#include "PressureTransducer.h"
#include "../lib/Sensors/XDBPressureTransducer.h"
#include "Gui.h"
#include "../lib/NextionGui.h"
// #include <avr8-stub.h>

#define PT100
#define FREQUENCY 50

#ifdef PT100
#define CSPIN 10
#include "../lib/Sensors/RTDTemperatureSensor.h"
TemperatureSensor *boilerTemp = new RTDTemperatureSensor(CSPIN, PT_100, 430,  MAX31865_4WIRE);
#endif

#define PRESSURE_TRANSDUCER_PIN A5
PressureTransducer *pumpPressure = new XDBPressureTransducer(PRESSURE_TRANSDUCER_PIN);

#define BREW_SWITCH_PIN A0
SwitchSensor *brewSwitch = new SimpleSwitch(BREW_SWITCH_PIN);


#define STEAM_PIN A2
SwitchSensor *steamSwitch = new SimpleSwitch(STEAM_PIN);

#define PUMP_DIMMER_PIN 9
#define PUMP_DIMMER_ZC_PIN 2
PumpController *pumpControl = new RBDPumpController(PUMP_DIMMER_PIN, PUMP_DIMMER_ZC_PIN, 127);


Gui *gui = new NextionGui();

#define BOILER_SSR_PIN 3
CoffeeMachine *coffeeMachine = new CoffeeMachine(*pumpControl, *steamSwitch, *brewSwitch, *boilerTemp, *gui, *pumpPressure, BOILER_SSR_PIN);


void setup()
{
  // debug_init();
  coffeeMachine->begin();
}

void loop()
{
  coffeeMachine->loop();
}


// Save button
void trigger1() {
  coffeeMachine->onSaveTriggered();
}