#include "PressureSensor.h"

PressureSensor::PressureSensor()
    : bmp(), bmp_pressure(bmp.getPressureSensor()),
      bmp_temp(bmp.getTemperatureSensor()) {
  sensorData.data_count = DATA_COUNT;
}

void PressureSensor::begin() {
  if (!bmp.begin()) {
    printErrorMessageToSerial(
        "no BMP280 detected ... Check your wiring or I2C ADDR!");
  }

  bmp.setSampling(
      Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
      Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
      Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
      Adafruit_BMP280::FILTER_X16,      /* Filtering. */
      Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  bmp_temp->printSensorDetails();
}

void PressureSensor::update() {
  sensors_event_t temp_event, pressure_event;
  bmp_temp->getEvent(&temp_event);
  bmp_pressure->getEvent(&pressure_event);
  sensorData.values[PRESSURE] = pressure_event.pressure;
  sensorData.values[TEMPERATURE] = temp_event.temperature;
  sensorData.values[ALTITUDE] = bmp.readAltitude(1013.25);
}

String PressureSensor::toString() const {
  return "Pressure: " + String(sensorData.values[PRESSURE]) +
         ", Temperature: " + String(sensorData.values[TEMPERATURE]) +
         ", Altitude: " + String(sensorData.values[ALTITUDE]);
}
