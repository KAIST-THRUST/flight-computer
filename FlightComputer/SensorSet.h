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

  int current_time = 0;
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
};

class SensorSet {
public:
  enum SensorType {
    GPS,
    IMU,
    BAROMETER,
    VOLTMETER,
    PRESSURE,
    SENSOR_COUNT
  };
  SensorSet();
  /* Add a sensor to the set. */
  void addSensor(Sensor *sensor, enum SensorType sensor_type);
  void addSensor(GPSSensor *sensor);
  void addSensor(IMUSensor *sensor);
  void addSensor(BarometerSensor *sensor);
  void addSensor(ADCSensor *sensor);

  void beginAll();  // Begin all sensors.
  void updateAll(); // Update all sensors.

  /* Getter for specific sensor data. */
  float getValue(enum GPSSensor::GPSDataIndex index) const;
  float getValue(enum IMUSensor::IMUDataIndex index) const;
  float getValue(enum BarometerSensor::BarometerDataIndex index) const;
  float getValue(enum ADCSensor::ADCDataIndex index) const;

  const SensorDataCollection &getSensorDataCollection() const {
    return sensor_data_collection;
  }

private:
  Sensor *sensors[SENSOR_COUNT];
  SensorDataCollection sensor_data_collection;
};

#endif
