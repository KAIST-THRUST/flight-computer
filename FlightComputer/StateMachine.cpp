#include "StateMachine.h"

enum RocketState rocket_current_state = INITIAL_ROCKET_STATE;

StateMachine::StateMachine()
    : sensor_data_collection(), sensor_set(sensor_data_collection),
      gps_latitude_ls(
          &sensor_data_collection.gps_data[GPSSensor::LATITUDE],
          &sensor_data_collection.gps_data[GPSSensor::LATITUDE_LS]),
      gps_longitude_ls(
          &sensor_data_collection.gps_data[GPSSensor::LONGITUDE],
          &sensor_data_collection.gps_data[GPSSensor::LONGITUDE_LS]),
      gps_altitude_ls(
          &sensor_data_collection.gps_data[GPSSensor::ALTITUDE],
          &sensor_data_collection.gps_data[GPSSensor::ALTITUDE_LS]),
      gps_geoid_height_ls(
          &sensor_data_collection.gps_data[GPSSensor::GEOID_HEIGHT],
          &sensor_data_collection.gps_data[GPSSensor::GEOID_HEIGHT_LS]),
      bar_pressure_avg(
          &sensor_data_collection
               .barometer_data[BarometerSensor::PRESSURE],
          &sensor_data_collection
               .barometer_data[BarometerSensor::PRESSURE_AVG]),
      bar_temperature_avg(
          &sensor_data_collection
               .barometer_data[BarometerSensor::TEMPERATURE],
          &sensor_data_collection
               .barometer_data[BarometerSensor::TEMPERATURE_AVG]) {}

void StateMachine::begin() {
  rtc.begin();
  Serial.begin(BAUD_RATE);
  Serial.println("Hello World!"); // serial monitor test.
  servo.begin();
  sensor_set.beginAll();
  delay(1000); // Wait for sensors to initialize.
  const TimeData &time_data = rtc.getTimeData();
  Serial.print(log_formatter.format(LogCategory::INFO, time_data));
  String file_name_time = String(time_data) + ".txt";
  Serial.println(" First rocket drop test.");
  sd_manager.begin(file_name_time);
  sd_manager.write("Hello World!"); // SD card test.
  sd_manager.close();
  delay(5000);
}

void StateMachine::boot() {
  sensor_set.gps_sensor.update();
  if (sensor_set.gps_sensor.isFixed()) {
    rocket_current_state = RocketState::ST_STAND_BY;
    initial_fix_time = 0;
    Serial.println(
        log_formatter.format(LogCategory::INFO, "All Sensors fixed."));
    Serial.println(
        log_formatter.format(LogCategory::INFO, "Switch to STAND_BY"));
  }
}

void StateMachine::standBy() {
  uint32_t current_time = millis();
  sensor_set.gps_sensor.update(); // GPS need to be updated every loop.
  if (current_time - sensor_data_collection.current_time >=
      (1000 / SAMPLING_RATE)) {
    /* Update sensor_data_collection every SAMPLING_RATE Hz. */
    sensor_data_collection.current_time = current_time;
    sensor_set.imu_sensor.update();
    sensor_set.barometer_sensor.update();
    sensor_set.adc_sensor.update();

    /* Updating average sensor values for 3 minutes. */
    if (initial_fix_time < 30 * 1000) {
      gps_latitude_ls.update();
      gps_longitude_ls.update();
      gps_altitude_ls.update();
      gps_geoid_height_ls.update();
      bar_pressure_avg.update();
      bar_temperature_avg.update();
    }

    /* Logging sensor data to Serial. */
    Serial.println(log_formatter.format(sensor_data_collection));
    sd_manager.write(log_formatter.format(sensor_data_collection));

    /* Writing raw data to SD card. */
    // sd_manager.write(log_formatter.format(sensor_data_collection));
  }
  if (initial_fix_time >= 30 * 1000) {
    navigation.initializeLaunchSiteConfig(
        sensor_data_collection.gps_data[GPSSensor::LATITUDE_LS],
        sensor_data_collection.gps_data[GPSSensor::LONGITUDE_LS],
        sensor_data_collection.gps_data[GPSSensor::ALTITUDE_LS],
        sensor_data_collection.gps_data[GPSSensor::GEOID_HEIGHT_LS],
        sensor_data_collection
            .barometer_data[BarometerSensor::PRESSURE_AVG],
        sensor_data_collection
            .barometer_data[BarometerSensor::TEMPERATURE_AVG]);
    rocket_current_state = RocketState::ST_BURN;
    Serial.println(
        log_formatter.format(LogCategory::INFO, "Switch to BURN"));
  }
}

void StateMachine::burn() {
  if (true) {
    rocket_current_state = RocketState::ST_COAST;
    Serial.println(
        log_formatter.format(LogCategory::INFO, "Switch to COAST"));
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

    /* Update and get navigation data from Navigation class. */
    navigation.update(sensor_data_collection);
    navigation_data.current_time = sensor_data_collection.current_time;
    navigation.getPosENU_m(navigation_data.pos_ENU);
    navigation.getVelENU_ms(navigation_data.vel_ENU);
    navigation_data.max_altitude =
        max(navigation_data.max_altitude, navigation_data.pos_ENU[2]);

    /* Logging sensor data to Serial. */
    Serial.println(log_formatter.format(sensor_data_collection));
    Serial.println(log_formatter.format(navigation_data));

    /* Writing raw data to SD card. */
    sd_manager.write(log_formatter.format(sensor_data_collection));
    sd_manager.write(log_formatter.format(navigation_data));
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
      (navigation_data.max_altitude - navigation_data.pos_ENU[2]) > 2.5;

  // bool isTimeToEject = millis() > 2 * 1000; // 2 seconds after launch
  return isDescenting;
}
