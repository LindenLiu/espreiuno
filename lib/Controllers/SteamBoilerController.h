#pragma once

#include "BoilerController.h"

class SteamBoilerController : public BoilerController<double>
{
private:
  double temperatureRange; // The temperature should be within the [target - range, target + range];
public:
  SteamBoilerController(const double &temperatureRange);
  ~SteamBoilerController();
  void begin() override;
  bool changeControlParams(const double &temperatureRange) override;
  const double& getControlParams() override;
  bool shouldBoilerOn(const double &target, const double &currentTemperature) override;
};

SteamBoilerController::SteamBoilerController(const double &temperatureRange) 
{
  changeControlParams(temperatureRange);
}

SteamBoilerController::~SteamBoilerController()
{
}

bool SteamBoilerController::changeControlParams(const double &temperatureRange) {
  this->temperatureRange = temperatureRange > 0 ? temperatureRange : -temperatureRange;
  return true;
}

const double& SteamBoilerController::getControlParams() 
{
  return this->temperatureRange;
}

bool SteamBoilerController::shouldBoilerOn(const double &target, const double &currentTemperature)
{
  // If the temperature lower than high and not in the range of low and high ( must be lower than low),
  // turn the boiler on.
  // If the temperature is in the range of [low, high], keep boiler off.
  // The currentTemperature must less than MAX_STEAM_TEMP_IN_C to be safe.
  return currentTemperature < MAX_STEAM_TEMP_IN_C && currentTemperature < target + temperatureRange && ! currentTemperature < target - temperatureRange;
}