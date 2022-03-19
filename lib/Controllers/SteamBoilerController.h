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
  bool shouldBoilerOn(const double &target, const double &currentTemperature) override;
};

SteamBoilerController::SteamBoilerController(const double &temperatureRange) 
{
  changeControlParams(temperatureRange);
}

SteamBoilerController::~SteamBoilerController()
{
}

void SteamBoilerController::begin() 
{

}

bool SteamBoilerController::changeControlParams(const double &temperatureRange) {
  this->temperatureRange = temperatureRange > 0 ? temperatureRange : -temperatureRange;
  return true;
}

bool SteamBoilerController::shouldBoilerOn(const double &target, const double &currentTemperature)
{
  // If the temperature lower than high and not in the range of low and high ( must be lower than low),
  // turn the boiler on.
  // If the temperature is in the range of [low, high], keep boiler off.
  // The currentTemperature must less than MAX_STEAM_TEMP_IN_C to be safe.
  return currentTemperature < MAX_STEAM_TEMP_IN_C && currentTemperature < target + temperatureRange && ! currentTemperature < target - temperatureRange;
}