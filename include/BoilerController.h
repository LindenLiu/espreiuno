#pragma once

#define MIN_SAMPLE_TIME 50
#define MAX_STEAM_TEMP_IN_C 150 // Gaggia's Steam thermostate's max.

typedef struct PIDParams
{
  double kP = 5;
  double kI = 3;
  double kD = 3;
  uint16_t sampleTime = 200;
} PIDParams_t;

template <typename ControlParameters>
class BoilerController
{
private:
public:
  BoilerController();
  ~BoilerController();
  virtual void begin();
  virtual bool changeControlParams(const ControlParameters &params);
  const virtual ControlParameters& getControlParams();
  virtual bool shouldBoilerOn(const double &target, const double &currentTemperature);
};

