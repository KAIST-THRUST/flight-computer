/**
 * @file IMU.h
 * @brief A IMU class that reads data from BNO 055 sensor.
 */

#ifndef IMUSENSOR_H
#define IMUSENSOR_H

#include "Sensor.h"
#include "config.h"
#include "util.h"
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <utility/imumaths.h>

class IMUSensor : public Sensor {
public:
  enum IMUDataIndex { DATA_COUNT };
  IMUSensor();
  void begin() override;
  void update() override;
  String toString() override;

private:
  Adafruit_BNO055 bno;
};

#endif
