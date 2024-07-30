#include "StateMachine.h"

enum RocketState rocket_current_state = INITIAL_ROCKET_STATE;

StateMachine::StateMachine()
    : sensor_set(sensor_data_collection),
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
               .barometer_data[BarometerSensor::TEMPERATURE_AVG]),
      hc12(HC12_SERIAL, &log_formatter) {}

void StateMachine::begin() {
  /* Get execution initial time. */
  rtc.begin();
  const TimeData &time_data = rtc.getTimeData();

  /* Create file name with current time. */
  char file_name_time[50];
  time_data.toString(file_name_time);

  /* Initialize main serial and print initial message. */
  Serial.begin(BAUD_RATE);
  Serial.print(log_formatter.format(LogCategory::INFO, file_name_time));
  Serial.print(
      log_formatter.format(LogCategory::INFO, "Start system."));

  servo.begin();         // Initialize servo motor.
  sensor_set.beginAll(); // Initialize all sensors.
  Serial.print(log_formatter.format(LogCategory::INFO,
                                    "All sensors initialized."));

  hc12.begin(); // Initialize HC12 communication module.

  /* Initialize SD card. */
  strcat(file_name_time, ".txt"); // Append .txt to file name.
  Serial.print(
      log_formatter.format(LogCategory::INFO, "Waiting for SD card."));
  /* Loop until SD card is properly initialized. */
  while (!sd_manager.begin(file_name_time)) {
    delay(1000);
  }
  Serial.print(
      log_formatter.format(LogCategory::INFO, "SD card initialized."));

  sd_manager.write("Static fire test.\n"); // SD card test.
  memcpy(hc12_buffer, &rocket_current_state, 1);
  delay(5000);
  Serial.print(
      log_formatter.format(LogCategory::INFO, "Switch to BOOT"));
  sd_manager.write(
      log_formatter.format(LogCategory::INFO, "Switch to BOOT"));
  since_boot = 0;
}

void StateMachine::boot() {
  sensor_set.gps_sensor.update();
  if (hc12_timer > 1000 / HC12_SAMPLING_RATE) {
    hc12.writeRaw(hc12_buffer, 1);
    hc12_timer -= 1000 / HC12_SAMPLING_RATE;
  }

  /* Check if all sensors are fixed. */
  if (sensor_set.isValid() || since_boot > S_TO_MS(30)) {
    rocket_current_state = RocketState::ST_STAND_BY;
    memcpy(hc12_buffer, &rocket_current_state, 1);
    since_fix = 0;
    Serial.print(
        log_formatter.format(LogCategory::INFO, "All Sensors fixed."));
    Serial.print(
        log_formatter.format(LogCategory::INFO, "Switch to STAND_BY"));
  }
}

void StateMachine::standBy() {
  uint32_t current_time = millis();
  sensor_set.gps_sensor.update(); // GPS has to be updated every loop.
  if (current_time - sensor_data_collection.current_time >=
      (1000 / SAMPLING_RATE)) {
    /* Update sensor_data_collection every SAMPLING_RATE Hz. */
    sensor_data_collection.current_time = current_time;
    sensor_set.imu_sensor.update();
    sensor_set.barometer_sensor.update();
    sensor_set.adc_sensor.update();

    /* Updating average sensor values for 30 seconds. */
    if (since_fix < S_TO_MS(30)) {
      updateAverageSensorData();
    }

    /* Logging sensor data. */
    // Serial.print(log_formatter.format(sensor_data_collection));
    sd_manager.write(log_formatter.format(sensor_data_collection));

    /* Construct HC12 buffer and write to HC12. */
    memcpy(hc12_buffer, &rocket_current_state, 1);
    // memcpy(hc12_buffer + 1, &sensor_data_collection.current_time, 4);
    // for (int i = 0; i < 8; i++) {
    //   memcpy(hc12_buffer + 5 + i * 4,
    //          &sensor_data_collection.gps_data[i], 4);
    // }
    // for (int i = 0; i < 10; i++) {
    //   memcpy(hc12_buffer + 37 + i * 4,
    //          &sensor_data_collection.imu_data[i], 4);
    // }
    // for (int i = 0; i < 4; i++) {
    //   memcpy(hc12_buffer + 77 + i * 4,
    //          &sensor_data_collection.barometer_data[i], 4);
    // }
    // for (int i = 0; i < 2; i++) {
    //   memcpy(hc12_buffer + 93 + i * 4,
    //          &sensor_data_collection.adc_data[i], 4);
    // }
    if (hc12_timer > 1000 / HC12_SAMPLING_RATE) {
      hc12.writeRaw(hc12_buffer, 1);
      hc12_timer -= 1000 / HC12_SAMPLING_RATE;
    }
  }

  /* Check if the rocket is in BURN state. */
  if (sensor_data_collection.adc_data[ADCSensor::PRESSURE] > 0.5 ||
      since_fix > S_TO_MS(30)) {
    since_burn = 0;
    initializeNavigation();
    rocket_current_state = RocketState::ST_BURN;
    Serial.print(
        log_formatter.format(LogCategory::INFO, "Switch to BURN"));
    sd_manager.write(
        log_formatter.format(LogCategory::INFO, "Switch to BURN"));
  }
}

void StateMachine::burn() {
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
    updateNavigation();

    /* Logging sensor data to Serial. */
    // Serial.print(log_formatter.format(sensor_data_collection));
    // Serial.print(log_formatter.format(navigation_data));

    /* Writing raw data to SD card. */
    sd_manager.write(log_formatter.format(sensor_data_collection));
    sd_manager.write(log_formatter.format(navigation_data));
  }

  /* Check if the rocket is in COAST state. */
  if (sensor_data_collection.adc_data[ADCSensor::PRESSURE] < 1.0 ||
      since_burn > S_TO_MS(3)) {
    rocket_current_state = RocketState::ST_COAST;
    Serial.print(
        log_formatter.format(LogCategory::INFO, "Switch to COAST"));
    sd_manager.write(
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
    updateNavigation();

    /* Logging sensor data to Serial. */
    // Serial.print(log_formatter.format(sensor_data_collection));
    // Serial.print(log_formatter.format(navigation_data));

    /* Writing raw data to SD card. */
    sd_manager.write(log_formatter.format(sensor_data_collection));
    sd_manager.write(log_formatter.format(navigation_data));
  }

  /* Check if the rocket is in DESCENT state. */
  if (shouldEject()) {
    if (!servo.isrotating()) {
      servo.rotate(90);
    }
    Serial.print(log_formatter.format(
        LogCategory::INFO,
        "Ejection mechanism activated. Switch to DESCENT"));
    sd_manager.write(log_formatter.format(
        LogCategory::INFO,
        "Ejection mechanism activated. Switch to DESCENT"));
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
    updateNavigation();

    /* Logging sensor data to Serial. */
    // Serial.print(log_formatter.format(sensor_data_collection));

    /* Writing raw data to SD card. */
    sd_manager.write(log_formatter.format(sensor_data_collection));
  }
}

bool StateMachine::shouldEject() {
  bool isDescenting =
      (navigation_data.max_altitude - navigation_data.pos_ENU[2]) > 2.5;

  bool isTimeToEject = since_burn > S_TO_MS(10);
  return isDescenting || isTimeToEject;
}

void StateMachine::initializeNavigation() {
  navigation.initializeLaunchSiteConfig(
      sensor_data_collection.gps_data[GPSSensor::LATITUDE_LS],
      sensor_data_collection.gps_data[GPSSensor::LONGITUDE_LS],
      sensor_data_collection.gps_data[GPSSensor::ALTITUDE_LS],
      sensor_data_collection.gps_data[GPSSensor::GEOID_HEIGHT_LS],
      sensor_data_collection
          .barometer_data[BarometerSensor::PRESSURE_AVG],
      sensor_data_collection
          .barometer_data[BarometerSensor::TEMPERATURE_AVG]);
}

void StateMachine::updateNavigation() {
  navigation.update(sensor_data_collection);
  navigation_data.current_time = sensor_data_collection.current_time;
  navigation.getPosENU_m(navigation_data.pos_ENU);
  navigation.getVelENU_ms(navigation_data.vel_ENU);
  navigation_data.max_altitude =
      max(navigation_data.max_altitude, navigation_data.pos_ENU[2]);
}

void StateMachine::updateAverageSensorData() {
  gps_latitude_ls.update();
  gps_longitude_ls.update();
  gps_altitude_ls.update();
  gps_geoid_height_ls.update();
  bar_pressure_avg.update();
  bar_temperature_avg.update();
}
