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
#include "Types.h"

class Gui
{
private:
  /* data */
public:
  Gui(/* args */);
  virtual ~Gui();
  virtual void begin();
  virtual void loop();
  virtual void setBrewSwitchState(SwitchState_t state);
  virtual void setSteamSwitchState(SwitchState_t state);
  virtual void setTargetTemperature(float temperature);
  virtual void setTargetSteamTemperature(float temperature);
  virtual float getTargetTemperature();
  virtual float getTargetSteamTemperature();
  virtual void setTemperature(float temperature);
  virtual void setPressure(float pressure);
  virtual void setBoilerState(int boilerPwm);
  virtual void setPumpPower(int percentage);
  virtual void setPidParam(PIDParams_t &params);
  virtual PIDParams_t getPidParam();
  virtual void setPidTuneProgress(int progress);
  virtual GuiPage_t getCurrentPage();
  virtual uint8_t getManualPressure();
  virtual void setPreinfusionParams(PreinfusionParams_t &piParams);
  virtual PreinfusionParams_t getPreinfusionParams();
};

Gui::Gui(/* args */)
{
}

Gui::~Gui()
{
}
