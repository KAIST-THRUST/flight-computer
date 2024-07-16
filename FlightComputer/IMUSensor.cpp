#include "IMUSensor.h"

IMUSensor::IMUSensor() : bno(55, IMU_I2C_ADDRESS, &IMU_WIRE) {
  sensor_data.data_count = DATA_COUNT;
}

void IMUSensor::begin(float *ptr) {
  data_ptr = ptr;
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
  data_ptr[ACC_X] = accelerometerData.acceleration.x;
  data_ptr[ACC_Y] = accelerometerData.acceleration.y;
  data_ptr[ACC_Z] = accelerometerData.acceleration.z;

  /* Get angular velocity data */
  data_ptr[ANG_VEL_X] = angVelocityData.gyro.x;
  data_ptr[ANG_VEL_Y] = angVelocityData.gyro.y;
  data_ptr[ANG_VEL_Z] = angVelocityData.gyro.z;

  /* Get quaternion data */
  imu::Quaternion quat = bno.getQuat();
  data_ptr[QUAT_X] = quat.x();
  data_ptr[QUAT_Y] = quat.y();
  data_ptr[QUAT_Z] = quat.z();
  data_ptr[QUAT_W] = quat.w();
}

String IMUSensor::toString() const {
  return "[IMU] Acceleration: (x:" + String(data_ptr[ACC_X], 7) +
         ", y:" + String(data_ptr[ACC_Y], 7) +
         ", z:" + String(data_ptr[ACC_Z], 7) + "), " +
         "\n      Angular velocity: (x:" +
         String(data_ptr[ANG_VEL_X], 7) +
         ", y:" + String(data_ptr[ANG_VEL_Y], 7) +
         ", z:" + String(data_ptr[ANG_VEL_Z], 7) + "), " +
         "\n      Quaternion: (x:" + String(data_ptr[QUAT_X], 7) +
         ", y:" + String(data_ptr[QUAT_Y], 7) +
         ", z:" + String(data_ptr[QUAT_Z], 7) +
         ", w:" + String(data_ptr[QUAT_W], 7) + ")";
}
