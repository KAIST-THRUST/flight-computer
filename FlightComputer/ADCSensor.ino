#include "ADCSensor.h"

ADCSensor::ADCSensor() : ads() { sensorData.data_count = DATA_COUNT; }

void ADCSensor::begin() {
  if (!ads.begin(ADS1X15_ADDRESS, &ADC_WIRE)) {
    printErrorMessageToSerial("ADS1115 not found!");
  }
  ads.setGain(GAIN_TWOTHIRDS);
  ads.setDataRate(RATE_ADS1115_860SPS);
}

void ADCSensor::update() {
  if (last_update_time < 1000 / ADC_SAMPLING_RATE) {
    return;
  }
  last_update_time = last_update_time - 1000 / ADC_SAMPLING_RATE;
  int16_t pressure_adc = ads.readADC_SingleEnded(ADC_PRESSURE_PIN);
  int16_t voltage_adc = ads.readADC_SingleEnded(ADC_VOLTAGE_PIN);
  sensorData.values[PRESSURE] =
      map(ads.computeVolts(pressure_adc), 1, 5, 0, 1000);
  sensorData.values[VOLTAGE] = ads.computeVolts(voltage_adc);
}

String ADCSensor::toString() const {
  return "[ADC] Pressure: " + String(sensorData.values[PRESSURE], 7) +
         ", Voltage: " + String(sensorData.values[VOLTAGE], 7);
}
