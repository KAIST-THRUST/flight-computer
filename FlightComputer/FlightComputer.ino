/**
 * @file FlightComputer.ino
 * @brief Main sketch file for execution of the flight computer.
 */

#include "Navigation.h"
#include "NonBlockingServo.h"
#include "RealTimeClock.h"
#include "SDManager.h"
#include "SensorSet.h"
#include "util.h"

/* Servo motor. */
static NonBlockingServo serv;

/* Sensors set. */
static SensorSet sensor_set;

/* SD card manager. */
static SDManager sd_manager;

/* Time variables for non-blocking delay. */
static uint32_t current_time;
static RealTimeClock rtc;

void setup() {
  rtc.begin();
  Serial.begin(BAUD_RATE);
  Serial.println("Hello World!"); // serial monitor test.
  serv.begin();
  sensor_set.beginAll();
  delay(1000); // Wait for sensors to initialize.
  sd_manager.begin(rtc.getTimeData());
  sd_manager.write("Hello World!"); // SD card test.
}

void loop() {
  /*-----------------------------------------------------------------*/
  /* Servo part. */
  updateServoFromSerial(serv);
  /*-----------------------------------------------------------------*/

  switch (rocket_current_state) {
  case RocketState::ST_STAND_BY:
    /* Stand by state. */
    break;

  case RocketState::ST_BURN:
    /* Burn state. */
    break;

  case RocketState::ST_COAST:
    /* Coast state. */
    current_time = millis();
    sensor_set.gps_sensor.update(); // Need to be updated every loop.
    if (current_time - sensor_set.sensor_data_collection.current_time >=
        (1000 / SAMPLING_RATE)) {
      /* Update every SAMPLING_RATE Hz. */
      sensor_set.sensor_data_collection.current_time = current_time;
      sensor_set.imu_sensor.update();
      sensor_set.barometer_sensor.update();
      sensor_set.adc_sensor.update();

      /* Logging sensor data to Serial. */
      Serial.println(sensor_set.sensor_data_collection);
      Serial.println("");

      /* Writing sensor data to SD card. */
      sd_manager.write(sensor_set.sensor_data_collection);
      sd_manager.write("");
    }
    break;

  case RocketState::ST_DESCENT:
    /* Descent state. */
    break;

  case RocketState::ST_LANDED:
    /* Landed state. */
    break;
  }
}
