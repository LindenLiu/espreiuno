#pragma once

typedef struct PIDParams
{
  double kP = 5;
  double kI = 3;
  double kD = 3;
  uint16_t sampleTime = 200;
} PIDParams_t;

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

typedef enum SwitchState {
  SWITCH_OFF = 0,
  SWITCH_ON = 1
} SwitchState_t;
