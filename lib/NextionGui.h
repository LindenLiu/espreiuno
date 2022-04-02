#pragma once
#include <EasyNextionLibrary.h>
#include "Gui.h"

#define DEFAULT_PRECISION 10

typedef struct NextionState 
{
  SwitchState_t brewState = SWITCH_OFF;
} NextionState_t;

class NextionGui : public Gui
{
private:
  /* data */
  EasyNex nex;
  NextionState_t stateUpateOnChangeOnly;

public:
  NextionGui(/* args */);
  ~NextionGui();
  void begin() override;
  void loop() override;
  void setBrewSwitchState(SwitchState_t state) override;
  void setSteamSwitchState(SwitchState_t state) override;
  void setBoilerState(int boilerPwmValue) override;
  void setTargetTemperature(float temperature) override;
  float getTargetTemperature() override;
  void setTargetSteamTemperature(float temperature) override; 
  float getTargetSteamTemperature() override;
  void setTemperature(float temperature) override;
  void setPressure(float pressure) override;
  void setPumpPower(int percentage) override;
  void setPidParam(PIDParams_t &params) override;
  PIDParams_t getPidParam() override;
  void setPidTuneProgress(int progress) override;
  GuiPage_t getCurrentPage() override;
  uint8_t getManualPressure() override;
  void setPreinfusionParams(PreinfusionParams_t &piParams) override;
  PreinfusionParams_t getPreinfusionParams() override;
};

NextionGui::NextionGui(/* args */): nex(Serial)
{
  
}

NextionGui::~NextionGui()
{
}

void NextionGui::begin()
{
  nex.begin(115200);
  while (nex.readNumber("safetyTempCheck") != 100 )
  {
   delay(500);
  }
  nex.lastCurrentPageId = nex.currentPageId;
}

void NextionGui::loop()
{
  nex.NextionListen();
}

void NextionGui::setBrewSwitchState(SwitchState_t state) 
{
  if (stateUpateOnChangeOnly.brewState != state) 
  {
    nex.writeNum("timerState", state);
    stateUpateOnChangeOnly.brewState = state;
  }
}

void NextionGui::setSteamSwitchState(SwitchState_t state)
{

}

void NextionGui::setBoilerState(int boilerPwmValue) 
{
  nex.writeNum("currentHPWR", boilerPwmValue); 
}

void NextionGui::setTemperature(float temperature) 
{
  nex.writeNum("currentTemp",int(temperature));
}

void NextionGui::setTargetTemperature(float temperature) 
{
  nex.writeNum("setPoint", temperature);
  nex.writeNum("moreTemp.n1.val", temperature); 
}

float NextionGui::getTargetTemperature()
{
  return nex.readNumber("setPoint");
}

void NextionGui::setTargetSteamTemperature(float temperature)
{
  nex.writeNum("moreTemp.steamTemp.val", temperature);
  nex.writeNum("steamTemperature", temperature);
}

float NextionGui::getTargetSteamTemperature()
{
  return nex.readNumber("steamTemperature");
}

void NextionGui::setPressure(float pressure) 
{
  nex.writeStr("t11.txt", String(pressure));
}

void NextionGui::setPumpPower(int percentage)
{

}

void NextionGui::setPidParam(PIDParams_t &params) 
{
  nex.writeNum("morePid.xKP.val", params.kP*DEFAULT_PRECISION);
  nex.writeNum("morePid.xKI.val", params.kI*DEFAULT_PRECISION);
  nex.writeNum("morePid.xKD.val", params.kD*DEFAULT_PRECISION); 
}

void NextionGui::setPidTuneProgress(int progress)
{
  nex.writeNum("morePid.progTune.val", min(100,progress));
}

PIDParams_t NextionGui::getPidParam()
{
  PIDParams_t p;
  p.kP = nex.readNumber("morePid.xKP.val") / float(DEFAULT_PRECISION);
  p.kD = nex.readNumber("morePid.xKD.val") / float(DEFAULT_PRECISION);
  p.kI = nex.readNumber("morePid.xKI.val") / float(DEFAULT_PRECISION);
  return p;
}

GuiPage_t NextionGui::getCurrentPage()
{
  return static_cast<GuiPage_t>(nex.currentPageId);
}

uint8_t NextionGui::getManualPressure()
{
  return nex.readNumber("currentPressure");
}

PreinfusionParams_t NextionGui::getPreinfusionParams() {
  PreinfusionParams_t params;
  params.bar = nex.readNumber("piBar");
  params.state = nex.readNumber("piState") == 0 ? SWITCH_OFF : SWITCH_ON;
  params.prefinfusionSecs = nex.readNumber("piSec");
  params.soakSecs = nex.readNumber("piSoak");
  return params;
}

void NextionGui::setPreinfusionParams(PreinfusionParams_t &params)
{
  nex.writeNum("piState", params.state);
  nex.writeNum("brewAuto.bt0.val", params.state);
  nex.writeNum("piSec", params.prefinfusionSecs);
  nex.writeNum("brewAuto.n0.val", params.prefinfusionSecs);
  nex.writeNum("piBar", params.bar);
  nex.writeNum("brewAuto.n1.val", params.bar);
  nex.writeNum("piSoak", params.soakSecs);
  nex.writeNum("brewAuto.n4.val", params.soakSecs);
}