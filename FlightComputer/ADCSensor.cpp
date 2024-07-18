#include "ADCSensor.h"

ADCSensor::ADCSensor() : ads(ADC_I2C_ADDRESS, &ADC_WIRE) {
  sensor_data.data_count = DATA_COUNT;
}

void ADCSensor::begin(float *ptr) {
  data_ptr = ptr;
  ADC_WIRE.begin();
  if (!ads.begin()) {
    printErrorMessageToSerial("ADS1115 not found!");
  }
  ads.setGain(0);
  ads.setMode(1);
  ads.setDataRate(7);
}

void ADCSensor::update() {
  last_update_time = last_update_time - 1000 / ADC_SAMPLING_RATE;
  int16_t pressure_adc = ads.readADC(ADC_PRESSURE_PIN);
  int16_t voltage_adc = ads.readADC(ADC_VOLTAGE_PIN);
  data_ptr[PRESSURE] =
      map(ads.toVoltage(pressure_adc), 1, 5, 0, 68.9476);
  data_ptr[VOLTAGE] = ads.toVoltage(voltage_adc);
}

String ADCSensor::toString() const {
  return "[ADC] Pressure: " + String(data_ptr[PRESSURE], 7) +
         ", Voltage: " + String(data_ptr[VOLTAGE], 7);
}
