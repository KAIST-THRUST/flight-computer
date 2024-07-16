/**
 * @file BarometerSensor.h
 * @brief A Barometer sensor class that reads data from BMP280 sensor.
 */

#ifndef BAROMETERSENSOR_H
#define BAROMETERSENSOR_H

#include "Sensor.h"
#include "config.h"
#include "util.h"
#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <Wire.h>

class BarometerSensor : public Sensor {
public:
  enum BarometerDataIndex {
    PRESSURE,        // in hPa
    PRESSURE_AVG,    // in hPa
    TEMPERATURE,     // in Celsius
    TEMPERATURE_AVG, // in Celsius
    DATA_COUNT
  };
  BarometerSensor();
  void begin(float *ptr) override;
  void update() override;
  String toString() const override;

private:
  Adafruit_BMP280 bmp;
  Adafruit_Sensor *bmp_pressure;
  Adafruit_Sensor *bmp_temp;
  AverageSensorValue pressure_avg;
  AverageSensorValue temperature_avg;
};

#endif
