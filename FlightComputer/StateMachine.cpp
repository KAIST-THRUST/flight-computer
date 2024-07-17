#include "StateMachine.h"

enum RocketState rocket_current_state = INITIAL_ROCKET_STATE;

StateMachine::StateMachine() : sensor_set(sensor_data_collection) {}

void StateMachine::begin() {
  rtc.begin();
  Serial.begin(BAUD_RATE);
  Serial.println("Hello World!"); // serial monitor test.
  servo.begin();
  sensor_set.beginAll();
  delay(1000); // Wait for sensors to initialize.
}

void StateMachine::coast() {
  uint32_t current_time = millis();
  sensor_set.gps_sensor.update(); // Need to be updated every loop.
  if (current_time - sensor_data_collection.current_time >=
      (1000 / SAMPLING_RATE)) {
    /* Update every SAMPLING_RATE Hz. */
    sensor_data_collection.current_time = current_time;
    sensor_set.imu_sensor.update();
    sensor_set.barometer_sensor.update();
    sensor_set.adc_sensor.update();

    /* Logging sensor data to Serial. */

    /* Writing raw data to SD card. */
  }

  if (shouldEject(sensor_data_collection)) {
    servo.rotate(90);
    rocket_current_state = RocketState::ST_DESCENT;
  }
}

bool StateMachine::shouldEject(
    SensorDataCollection &sensor_data_collection) {
  return true;
}
