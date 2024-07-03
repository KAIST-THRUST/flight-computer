/**
 * @file VoltmeterSensor.h
 * @brief
 */

#ifndef VOLTMETERSENSOR_H
#define VOLTMETERSENSOR_H

#include "Sensor.h"
#include "config.h"
#include "util.h"
#include <Adafruit_ADS1X15.h>

class VoltmeterSensor : public Sensor {
public:
  enum VoltmeterDataIndex { VOLTAGE, DATA_COUNT };
  VoltmeterSensor();
  void begin() override;
  void update() override;
  String toString() const override;

private:
  Adafruit_ADS1115 ads;
};

#endif
