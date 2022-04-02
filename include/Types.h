#pragma once

typedef struct PIDParams
{
  float kP = 0.2;
  float kI = 0.1;
  float kD = 0;
  uint16_t sampleTime = 100;
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

typedef struct PreinfusionParams 
{
  uint8_t prefinfusionSecs = 5;
  uint8_t soakSecs = 5;
  uint8_t bar=2;
  SwitchState_t state=SWITCH_OFF;
} PreinfusionParams_t;

typedef struct CoffeeMachineConfig
{
  float targetBrewTemp = 90;
  float targetSteamTemp = 142;
  PreinfusionParams_t preinfusionConfig;
  PIDParams_t pidParams;

  // Must be last one.
  uint32_t crc = 0; 
} CoffeeMachineConfig_t;

typedef enum GuiPage {
  HOME=0,
  BREWING_AUTO=1,
  BREWING_MANUAL=2,
  SETTINGS_BREW=3,
  CLEAN_FLUSH=4,
  CLEAN_DESCALE=5,
  SETTINGS_TEMP=6,
  SETTINGS_POWER=7,
  POP_MSG=8,
  SETTINGS_PID=15
} GuiPage_t;
