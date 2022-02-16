#pragma once

#define SECOND_IN_MS 1000

typedef enum BrewState {
  PREINFUSION_START = 0,
  PREINFUSION_SOAK = 1,
  PREINFUSION_BREW = 2,
  PREINFUSION_END = 3
} BrewState_t;

typedef struct BrewStep {
  uint8_t lengthInSeconds; // The brew cycle should be less than 255 seconds.
  uint8_t pressureBar; // The desired presure in Bars. 
} PressureProfileStep_t;

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