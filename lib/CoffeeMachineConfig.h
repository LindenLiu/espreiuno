#pragma once
#include <Types.h>

typedef struct BrewingControlConfig
{
  double targetTemperature = 90;
  PIDParams_t pidParams;
  
} BrewingControlConfig_t;

typedef struct SteamControlConfig
{
  double targetTemperature = 142;
  double range = 1;
} SteamControlConfig_t;


typedef struct CoffeeMachineConfig
{
  BrewingControlConfig_t brewConfig;

} CoffeeMachineConfig_t;
