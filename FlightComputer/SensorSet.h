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
  int current_time;
  float sensor_data[GPSSensor::DATA_COUNT + IMUSensor::DATA_COUNT +
                    BarometerSensor::DATA_COUNT +
                    ADCSensor::DATA_COUNT]; // Sensor data array.
  float *gps_data = sensor_data;
  float *imu_data = sensor_data + GPSSensor::DATA_COUNT;
  float *barometer_data =
      sensor_data + GPSSensor::DATA_COUNT + IMUSensor::DATA_COUNT;
  float *adc_data = sensor_data + GPSSensor::DATA_COUNT +
                    IMUSensor::DATA_COUNT + BarometerSensor::DATA_COUNT;
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
