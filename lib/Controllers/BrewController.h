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

#include <Types.h>

#define SECOND_IN_MS 1000

class BrewController
{
private:
  // Array length of 3.
  BrewStep *preinfusionProfile;
  BrewStep *pressureProfile;
  BrewState_t brewState;
  unsigned long brewingStartTime;
  uint8_t preinfusionBrewStart;
  uint8_t currentStep;
  uint16_t currentStepTimeLimit;
public:
  BrewController(/* args */);
  ~BrewController();
  uint8_t calculatePresureBar(double &pressure);
  BrewState_t getBrewState();
  unsigned long brewingStartTimeMs();
};

BrewController::BrewController(/* args */) : currentStep(0)
{
  brewingStartTime = millis();
  preinfusionBrewStart = pressureProfile[PREINFUSION_START].lengthInSeconds + pressureProfile[PREINFUSION_SOAK].lengthInSeconds;
  currentStepTimeLimit = NULL != pressureProfile ? pressureProfile[0].lengthInSeconds : 0;
}

BrewController::~BrewController()
{

}

uint8_t BrewController::calculatePresureBar(double &pressure) 
{
  if (brewState == PREINFUSION_END) 
  {
    return 0;
  }

  uint8_t currentSeconds = (millis() - brewingStartTime) / SECOND_IN_MS;
  // Calculate brew state

  if (NULL == pressureProfile || brewState < PREINFUSION_BREW) {
    return preinfusionProfile[brewState].pressureBar;
  }
 
  uint8_t pressureProfileLen = sizeof(pressureProfile) / sizeof(BrewStep);
  // calculate current step
  while (currentSeconds - preinfusionBrewStart > currentStepTimeLimit) {
    currentStep ++;
    if (currentStep < pressureProfileLen) 
    {
      currentStepTimeLimit += pressureProfile[currentStep].lengthInSeconds;
    } else {
      brewState = PREINFUSION_END;
      return 0;
    }
  }

  return pressureProfile[currentStep].pressureBar;

}

BrewState_t BrewController::getBrewState() 
{
  return brewState;
}

unsigned long BrewController::brewingStartTimeMs() 
{
  return brewingStartTime;
}