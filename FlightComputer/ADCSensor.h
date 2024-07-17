/**
 * @file ADCSensor.h
 * @brief A ADC sensor class that reads data from ADS1115 sensor.
 */

#ifndef ADCSENSOR_H
#define ADCSENSOR_H

#include "Sensor.h"
#include "config.h"
#include "util.h"
#include <ADS1X15.h>
#include <SPI.h>
#include <Wire.h>

class ADCSensor : public Sensor {
public:
  enum ADCDataIndex {
    VOLTAGE,  // in V
    PRESSURE, // in barg
    DATA_COUNT
  };
  ADCSensor();
  void begin(float *ptr) override;
  void update() override;
  String toString() const override;

private:
  ADS1115 ads;
  elapsedMillis last_update_time;
};

#endif
