#include "IMUSensor.h"

IMUSensor::IMUSensor() {
  bno = Adafruit_BNO055(55, IMU_I2C_ADDRESS, &IMU_WIRE);
  sensorData.data_count = DATA_COUNT;
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
  bno.getEvent(&accelerometerData,
               Adafruit_BNO055::VECTOR_ACCELEROMETER);

  /* Get acceleration data */
  sensorData.values[ACC_X] = accelerometerData.acceleration.x;
  sensorData.values[ACC_Y] = accelerometerData.acceleration.y;
  sensorData.values[ACC_Z] = accelerometerData.acceleration.z;

  /* Get angular velocity data */
  sensorData.values[ANG_VEL_X] = angVelocityData.gyro.x;
  sensorData.values[ANG_VEL_Y] = angVelocityData.gyro.y;
  sensorData.values[ANG_VEL_Z] = angVelocityData.gyro.z;

  /* Get quaternion data */
  imu::Quaternion quat = bno.getQuat();
  sensorData.values[QUAT_X] = quat.x();
  sensorData.values[QUAT_Y] = quat.y();
  sensorData.values[QUAT_Z] = quat.z();
  sensorData.values[QUAT_W] = quat.w();
}

String IMUSensor::toString() const {
  return "[IMU] Acceleration: (x:" +
         String(sensorData.values[ACC_X], 7) +
         ", y:" + String(sensorData.values[ACC_Y], 7) +
         ", z:" + String(sensorData.values[ACC_Z], 7) + "), " +
         "\n      Angular velocity: (x:" +
         String(sensorData.values[ANG_VEL_X], 7) +
         ", y:" + String(sensorData.values[ANG_VEL_Y], 7) +
         ", z:" + String(sensorData.values[ANG_VEL_Z], 7) + "), " +
         "\n      Quaternion: (x:" +
         String(sensorData.values[QUAT_X], 7) +
         ", y:" + String(sensorData.values[QUAT_Y], 7) +
         ", z:" + String(sensorData.values[QUAT_Z], 7) +
         ", w:" + String(sensorData.values[QUAT_W], 7) + ")";
}
