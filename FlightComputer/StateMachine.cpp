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
  const TimeData &time_data = rtc.getTimeData();
  Serial.print(log_formatter.format(LogCategory::INFO, time_data));
  Serial.println(" First rocket drop test.");
  // sd_manager.begin(String(time_data) + ".txt");
  // sd_manager.write("Hello World!"); // SD card test.
  delay(5000);
}

void StateMachine::standBy() {
  if (true) {
    rocket_current_state = RocketState::ST_BURN;
  }
}

void StateMachine::burn() {
  if (true) {
    rocket_current_state = RocketState::ST_COAST;
  }
}

void StateMachine::coast() {
  uint32_t current_time = millis();
  sensor_set.gps_sensor.update(); // Need to be updated every loop.
  if (current_time - sensor_data_collection.current_time >=
      (1000 / SAMPLING_RATE)) {
    /* Update sensor_data_collection every SAMPLING_RATE Hz. */
    sensor_data_collection.current_time = current_time;
    sensor_set.imu_sensor.update();
    sensor_set.barometer_sensor.update();
    sensor_set.adc_sensor.update();

    // navigation.update(sensor_data_collection);

    /* Logging sensor data to Serial. */
    Serial.println(log_formatter.format(sensor_data_collection));

    /* Writing raw data to SD card. */
    // sd_manager.write(log_formatter.format(sensor_data_collection));
  }

  if (shouldEject(sensor_data_collection, navigation_data)) {
    if (!servo.isrotating()) {
      servo.rotate(90);
    }
    Serial.println(log_formatter.format(
        LogCategory::INFO, "Ejection mechanism activated."));
    sd_manager.write(log_formatter.format(
        LogCategory::INFO, "Ejection mechanism activated."));
    rocket_current_state = RocketState::ST_DESCENT;
  }
}

void StateMachine::descend() {
  uint32_t current_time = millis();
  sensor_set.gps_sensor.update(); // Need to be updated every loop.
  if (current_time - sensor_data_collection.current_time >=
      (1000 / SAMPLING_RATE)) {
    /* Update sensor_data_collection every SAMPLING_RATE Hz. */
    sensor_data_collection.current_time = current_time;
    sensor_set.imu_sensor.update();
    sensor_set.barometer_sensor.update();
    sensor_set.adc_sensor.update();

    /* Navigation data update. */
    navigation.update(sensor_data_collection);

    /* Logging sensor data to Serial. */
    Serial.println(log_formatter.format(sensor_data_collection));

    /* Writing raw data to SD card. */
    sd_manager.write(log_formatter.format(sensor_data_collection));
  }
}

bool StateMachine::shouldEject(
    SensorDataCollection &sensor_data_collection,
    NavigationData &navigation_data) {
  bool isDescenting =
      (navigation_data.max_altitude - navigation_data.alt_ENU) > 2.5;

  // bool isTimeToEject = millis() > 2 * 1000; // 2 seconds after launch
  return isDescenting;
}
