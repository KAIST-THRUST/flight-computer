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
      hc12(HC12_SERIAL, &log_formatter), sd_device(&log_formatter) {}

void StateMachine::begin() {
  /* Get execution initial time. */
  rtc.begin();
  const TimeData &time_data = rtc.getTimeData();

  /* Create file name with current time. */
  char file_name_time[50];
  time_data.toString(file_name_time);

  /* Initialize built-in LED for status check. */
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // LED is on until standby state.

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
  sd_device.setFileName(file_name_time);
  sd_device.begin();

  Serial.print(
      log_formatter.format(LogCategory::INFO, "SD card initialized."));

  // sd_manager.write("Static fire test.\n"); // SD card test.
  sd_device.write(LogCategory::INFO, "Static fire test.\n");
  memcpy(hc12_buffer, &rocket_current_state, 1);
  delay(5000);
  Serial.print(
      log_formatter.format(LogCategory::INFO, "Switch to BOOT"));
  sd_device.write(LogCategory::INFO, "Switch to BOOT");
  since_boot = 0;

#ifdef FC_TEST_ENABLED
  navigation_data.pos_ENU[2] = 3.14f; // For testing.
#endif
}

void StateMachine::boot() {
  sensor_set.gps_sensor.update();
  sendDataToHc12();

  /* Check if all sensors are fixed. */
  if (sensor_set.isValid()) {
    rocket_current_state = RocketState::ST_STAND_BY;
    digitalWrite(LED_BUILTIN, LOW); // Turn off LED.
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
#ifdef FC_DEBUG_ENABLED
    Serial.print(log_formatter.format(sensor_data_collection));
#endif
    sd_device.write(sensor_data_collection);

    /* Construct HC12 buffer and write to HC12. */
    sendDataToHc12();
  }

  /* Check if the rocket is in BURN state. */
  if (shouldChangeToBurn()) {
    since_burn = 0;
    initializeNavigation();
    rocket_current_state = RocketState::ST_BURN;
    Serial.print(
        log_formatter.format(LogCategory::INFO, "Switch to BURN"));
    sd_device.write(LogCategory::INFO, "Switch to BURN");
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
#ifdef FC_DEBUG_ENABLED
    Serial.print(log_formatter.format(sensor_data_collection));
    Serial.print(log_formatter.format(navigation_data));
#endif
    /* Writing raw data to SD card. */
    sd_device.write(sensor_data_collection);
    sd_device.write(navigation_data);

    /* Sending raw data using HC12. */
    sendDataToHc12();
  }

  /* Check if the rocket is in COAST state. */
  if (shouldChangeToCoast()) {
    rocket_current_state = RocketState::ST_COAST;
    Serial.print(
        log_formatter.format(LogCategory::INFO, "Switch to COAST"));
    sd_device.write(LogCategory::INFO, "Switch to COAST");
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
#ifdef FC_DEBUG_ENABLED
    Serial.print(log_formatter.format(sensor_data_collection));
    Serial.print(log_formatter.format(navigation_data));
#endif
    /* Writing raw data to SD card. */
    sd_device.write(sensor_data_collection);
    sd_device.write(navigation_data);

    /* Sending raw data using HC12. */
    sendDataToHc12();
  }

  /* Check if the rocket is in DESCENT state. */
  if (shouldEject()) {
    if (!servo.isrotating()) {
      servo.rotate(90);
    }
    Serial.print(log_formatter.format(
        LogCategory::INFO,
        "Ejection mechanism activated. Switch to DESCENT"));
    sd_device.write(LogCategory::INFO,
                    "Ejection mechanism activated. Switch to DESCENT");
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
#ifdef FC_DEBUG_ENABLED
    Serial.print(log_formatter.format(sensor_data_collection));
#endif
    /* Writing raw data to SD card. */
    sd_device.write(sensor_data_collection);
    sd_device.write(navigation_data);

    /* Sending raw data using HC12. */
    sendDataToHc12();
  }
}

bool StateMachine::shouldChangeToBurn() {
#ifdef FC_TEST_ENABLED
  return since_fix > S_TO_MS(30);
#else
  return sensor_data_collection.adc_data[ADCSensor::PRESSURE] > 0.5;
#endif
}

bool StateMachine::shouldChangeToCoast() {
#ifdef FC_TEST_ENABLED
  return true;
#else
  return sensor_data_collection.adc_data[ADCSensor::PRESSURE] < 1.0;
#endif
}

bool StateMachine::shouldEject() {
  bool isDescenting =
      (navigation_data.max_altitude - navigation_data.pos_ENU[2]) > 2.5;

#ifdef FC_TEST_ENABLED
  return isDescenting;
#else
  bool isTimeToEject = since_burn > S_TO_MS(10);
  return isDescenting || isTimeToEject;
#endif
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

void StateMachine::sendDataToHc12() {
  if (since_transmit > 1000 / HC12_SAMPLING_RATE) {
    memcpy(hc12_buffer, &rocket_current_state, 1);
    memcpy(hc12_buffer + 1, &navigation_data.pos_ENU[2], 4);

    hc12.writeRaw(hc12_buffer, 5);
    since_transmit -= 1000 / HC12_SAMPLING_RATE;
  }
}

void StateMachine::updateSd() { sd_device.update(); }
