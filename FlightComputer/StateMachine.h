#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "Navigation.h"
#include "NonBlockingServo.h"
#include "RealTimeClock.h"
#include "SensorSet.h"
#include "config.h"

class StateMachine {
public:
  StateMachine();
  void begin();
  void standBy();
  void burn();
  void coast();
  void descend();
  void landed();

private:
  NonBlockingServo servo; // Servo motor for ejection mechanism.
  SensorSet sensor_set;   // Sensor set for reading data from sensors.
  SensorDataCollection sensor_data_collection; // Sensor data stored.
  NavigationData navigation_data; // Navigation data stored.
  RealTimeClock rtc;              // Real time clock for time data.

  /* Helper functions for state transition condition check. */
  bool shouldEject(SensorDataCollection &sensor_data_collection);
};

#endif
