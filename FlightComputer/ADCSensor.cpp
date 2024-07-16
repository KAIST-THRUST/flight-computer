#include "ADCSensor.h"

ADCSensor::ADCSensor() : ads() { sensor_data.data_count = DATA_COUNT; }

void ADCSensor::begin(float *ptr) {
  data_ptr = ptr;
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
  data_ptr[PRESSURE] =
      map(ads.computeVolts(pressure_adc), 1, 5, 0, 68.9476);
  data_ptr[VOLTAGE] = ads.computeVolts(voltage_adc);
}

String ADCSensor::toString() const {
  return "[ADC] Pressure: " + String(data_ptr[PRESSURE], 7) +
         ", Voltage: " + String(data_ptr[VOLTAGE], 7);
}
