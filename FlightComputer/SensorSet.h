/**
 * @file SensorSet.h
 * @brief A sensor set class that manages multiple sensors.
 */

#ifndef SENSORSET_H
#define SENSORSET_H

#include "ADCSensor.h"
#include "BarometerSensor.h"
#include "GPSSensor.h"
#include "IMUSensor.h"
#include "Sensor.h"
#include "config.h"

struct SensorDataCollection {
  static constexpr int TOTAL_DATA_COUNT =
      GPSSensor::DATA_COUNT + IMUSensor::DATA_COUNT +
      BarometerSensor::DATA_COUNT + ADCSensor::DATA_COUNT;

  uint32_t current_time = 0;
  float sensor_data[TOTAL_DATA_COUNT] = {0}; // All set to zero.
  float *gps_data;
  float *imu_data;
  float *barometer_data;
  float *adc_data;

  /* Constructor to initialize pointers. */
  SensorDataCollection()
      : gps_data(sensor_data),
        imu_data(sensor_data + GPSSensor::DATA_COUNT),
        barometer_data(sensor_data + GPSSensor::DATA_COUNT +
                       IMUSensor::DATA_COUNT),
        adc_data(sensor_data + GPSSensor::DATA_COUNT +
                 IMUSensor::DATA_COUNT + BarometerSensor::DATA_COUNT) {}

  operator String() const {
    char buffer[400];
    sprintf(
        buffer,
        "Time: %lu\n"
        "[GPS] Latitude: %.7f, Longitude: %.7f, Altitude: %.7f\n"
        "[IMU] Acceleration: (%.7f, %.7f, %.7f)\n"
        "      Angular velocity: (%.7f, %.7f, %.7f)\n"
        "      Quaternion: (%.7f, %.7f, %.7f, %.7f)\n"
        "[Barometer] Pressure: %.7f, Temperature: %.7f\n"
        "[ADC] Voltage: %.7f, Pressure: %.7f",
        current_time, gps_data[GPSSensor::LATITUDE],
        gps_data[GPSSensor::LONGITUDE], gps_data[GPSSensor::ALTITUDE],
        imu_data[IMUSensor::ACC_X], imu_data[IMUSensor::ACC_Y],
        imu_data[IMUSensor::ACC_Z], imu_data[IMUSensor::ANG_VEL_X],
        imu_data[IMUSensor::ANG_VEL_Y], imu_data[IMUSensor::ANG_VEL_Z],
        imu_data[IMUSensor::QUAT_X], imu_data[IMUSensor::QUAT_Y],
        imu_data[IMUSensor::QUAT_Z], imu_data[IMUSensor::QUAT_W],
        barometer_data[BarometerSensor::PRESSURE],
        barometer_data[BarometerSensor::TEMPERATURE],
        adc_data[ADCSensor::VOLTAGE], adc_data[ADCSensor::PRESSURE]);
    return String(buffer);
  }
};

struct SensorSet {
  /* Sensors to receive data. */
  IMUSensor imu_sensor;
  GPSSensor gps_sensor;
  BarometerSensor barometer_sensor;
  ADCSensor adc_sensor;

  /* Sensor data collection. */
  SensorDataCollection &sensor_data_collection;

  SensorSet(SensorDataCollection &sensor_data_collection)
      : sensor_data_collection(sensor_data_collection) {}

  void beginAll() {
    imu_sensor.begin(sensor_data_collection.imu_data);
    gps_sensor.begin(sensor_data_collection.gps_data);
    barometer_sensor.begin(sensor_data_collection.barometer_data);
    adc_sensor.begin(sensor_data_collection.adc_data);
    delay(1000); // Wait for sensors to initialize.
  }

  bool isValid() {
    return gps_sensor.isValid();
  }
};

#endif
