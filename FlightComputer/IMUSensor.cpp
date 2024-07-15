#include "IMUSensor.h"

IMUSensor::IMUSensor() : bno(55, IMU_I2C_ADDRESS, &IMU_WIRE) {
  sensor_data.data_count = DATA_COUNT;
}

void IMUSensor::begin() {
  if (!bno.begin(OPERATION_MODE_NDOF)) {
    printErrorMessageToSerial(
        "no BNO055 detected ... Check your wiring or I2C ADDR!");
  }
}

void IMUSensor::update() {
  sensors_event_t angVelocityData, accelerometerData;
  bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_LINEARACCEL);

  /* Get acceleration data */
  sensor_data.values[ACC_X] = accelerometerData.acceleration.x;
  sensor_data.values[ACC_Y] = accelerometerData.acceleration.y;
  sensor_data.values[ACC_Z] = accelerometerData.acceleration.z;

  /* Get angular velocity data */
  sensor_data.values[ANG_VEL_X] = angVelocityData.gyro.x;
  sensor_data.values[ANG_VEL_Y] = angVelocityData.gyro.y;
  sensor_data.values[ANG_VEL_Z] = angVelocityData.gyro.z;

  /* Get quaternion data */
  imu::Quaternion quat = bno.getQuat();
  sensor_data.values[QUAT_X] = quat.x();
  sensor_data.values[QUAT_Y] = quat.y();
  sensor_data.values[QUAT_Z] = quat.z();
  sensor_data.values[QUAT_W] = quat.w();
}

String IMUSensor::toString() const {
  return "[IMU] Acceleration: (x:" +
         String(sensor_data.values[ACC_X], 7) +
         ", y:" + String(sensor_data.values[ACC_Y], 7) +
         ", z:" + String(sensor_data.values[ACC_Z], 7) + "), " +
         "\n      Angular velocity: (x:" +
         String(sensor_data.values[ANG_VEL_X], 7) +
         ", y:" + String(sensor_data.values[ANG_VEL_Y], 7) +
         ", z:" + String(sensor_data.values[ANG_VEL_Z], 7) + "), " +
         "\n      Quaternion: (x:" +
         String(sensor_data.values[QUAT_X], 7) +
         ", y:" + String(sensor_data.values[QUAT_Y], 7) +
         ", z:" + String(sensor_data.values[QUAT_Z], 7) +
         ", w:" + String(sensor_data.values[QUAT_W], 7) + ")";
}
