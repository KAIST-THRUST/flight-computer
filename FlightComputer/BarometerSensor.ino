#include "BarometerSensor.h"

BarometerSensor::BarometerSensor()
    : bmp(&BAROMETER_WIRE), bmp_pressure(bmp.getPressureSensor()),
      bmp_temp(bmp.getTemperatureSensor()) {
  sensorData.data_count = DATA_COUNT;
}

void BarometerSensor::begin() {
  if (!bmp.begin(BAROMETER_I2C_ADDRESS)) {
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

void BarometerSensor::update() {
  sensors_event_t temp_event, pressure_event;
  bmp_temp->getEvent(&temp_event);
  bmp_pressure->getEvent(&pressure_event);
  sensorData.values[PRESSURE] = pressure_event.pressure;
  sensorData.values[TEMPERATURE] = temp_event.temperature;
  sensorData.values[ALTITUDE] = bmp.readAltitude(SEA_LEVEL_HPA);
  if (millis() <= 60 * 1000) {
    pressure_avg.addValue(sensorData.values[PRESSURE]);
    sensorData.values[PRESSURE_AVG] = pressure_avg.getAverage();
  }
}

String BarometerSensor::toString() const {
  return "[Barometer] Pressure: " +
         String(sensorData.values[PRESSURE], 7) +
         ", Average Pressure: " +
         String(sensorData.values[PRESSURE_AVG], 7) +
         ", Temperature: " + String(sensorData.values[TEMPERATURE], 7) +
         ", Altitude: " + String(sensorData.values[ALTITUDE], 7);
}
