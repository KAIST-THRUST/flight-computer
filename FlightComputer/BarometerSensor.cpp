#include "BarometerSensor.h"

BarometerSensor::BarometerSensor()
    : bmp(&BAROMETER_WIRE), bmp_pressure(bmp.getPressureSensor()),
      bmp_temp(bmp.getTemperatureSensor()) {
  sensor_data.data_count = DATA_COUNT;
}

void BarometerSensor::begin(float *ptr) {
  data_ptr = ptr;
  if (!bmp.begin(BAROMETER_I2C_ADDRESS)) {
    printErrorMessageToSerial(
        "no BMP280 detected ... Check your wiring or I2C ADDR!");
  }

  bmp.setSampling(
      Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
      Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
      Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
      Adafruit_BMP280::FILTER_X16,      /* Filtering. */
      Adafruit_BMP280::STANDBY_MS_125); /* Standby time. */

  // bmp_temp->printSensorDetails();
}

void BarometerSensor::update() {
  sensors_event_t temp_event, pressure_event;
  bmp_temp->getEvent(&temp_event);
  bmp_pressure->getEvent(&pressure_event);
  data_ptr[PRESSURE] = pressure_event.pressure;
  data_ptr[TEMPERATURE] = temp_event.temperature;
}

String BarometerSensor::toString() const {
  return "[Barometer] Pressure: " + String(data_ptr[PRESSURE], 7) +
         ", Average Pressure: " + String(data_ptr[PRESSURE_AVG], 7) +
         ", Temperature: " + String(data_ptr[TEMPERATURE], 7);
}
