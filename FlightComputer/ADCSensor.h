/**
 * @file ADCSensor.h
 * @brief A ADC sensor class that reads data from ADS1115 sensor.
 */

#ifndef ADCSENSOR_H
#define ADCSENSOR_H

#include "Sensor.h"
#include "config.h"
#include "util.h"
#include <Adafruit_ADS1X15.h>

class ADCSensor : public Sensor {
public:
  enum ADCDataIndex {
    VOLTAGE,  // in V
    PRESSURE, // in psig
    DATA_COUNT
  };
  ADCSensor();
  void begin() override;
  void update() override;
  String toString() const override;

private:
  Adafruit_ADS1115 ads;
};

#endif
