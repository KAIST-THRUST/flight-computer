/**
 * @file IMUSensor.h
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
  enum IMUDataIndex {
    ACC_X,     // x axis acceleration, in m/s^2
    ACC_Y,     // y axis acceleration, in m/s^2
    ACC_Z,     // z axis acceleration, in m/s^2
    ANG_VEL_X, // x axis angular velocity, in rad/s
    ANG_VEL_Y, // y axis angular velocity, in rad/s
    ANG_VEL_Z, // z axis angular velocity, in rad/s
    QUAT_X,    // x axis quaternion
    QUAT_Y,    // y axis quaternion
    QUAT_Z,    // z axis quaternion
    QUAT_W,    // w axis quaternion
    DATA_COUNT
  };
  IMUSensor();
  void begin() override;
  void update() override;
  String toString() const override;

private:
  Adafruit_BNO055 bno;
};

#endif
