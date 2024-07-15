#include "BarometerSensor.h"

BarometerSensor::BarometerSensor()
    : bmp(&BAROMETER_WIRE), bmp_pressure(bmp.getPressureSensor()),
      bmp_temp(bmp.getTemperatureSensor()) {
  sensor_data.data_count = DATA_COUNT;
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
  sensor_data.values[PRESSURE] = pressure_event.pressure;
  sensor_data.values[TEMPERATURE] = temp_event.temperature;
  if (millis() <= 180 * 1000 &&
      tu_1_current_state == state::ST_STAND_BY) {
    pressure_avg.addValue(sensor_data.values[PRESSURE]);
    sensor_data.values[PRESSURE_AVG] = pressure_avg.getAverage();
    temperature_avg.addValue(sensor_data.values[TEMPERATURE]);
    sensor_data.values[TEMPERATURE_AVG] = temperature_avg.getAverage();
  }
}

String BarometerSensor::toString() const {
  return "[Barometer] Pressure: " +
         String(sensor_data.values[PRESSURE], 7) +
         ", Average Pressure: " +
         String(sensor_data.values[PRESSURE_AVG], 7) +
         ", Temperature: " + String(sensor_data.values[TEMPERATURE], 7);
}
