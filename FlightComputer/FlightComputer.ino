/**
 * @file FlightComputer.ino
 * @brief Main sketch file for execution of the flight computer.
 */

#include "NonBlockingServo.h"
#include "RealTimeClock.h"
#include "SDManager.h"
#include "SensorSet.h"
#include "util.h"

/* Servo motor. */
static NonBlockingServo serv;

/* Sensors to receive data. */
static IMUSensor imu_sensor;
static GPSSensor gps_sensor;
static BarometerSensor barometer_sensor;
static ADCSensor adc_sensor;

/* SD card manager. */
static SDManager sd_manager;

/* Time variables for non-blocking delay. */
static uint32_t last_update_time = 0;
static uint32_t current_time;
static RealTimeClock rtc;

void setup() {
  rtc.begin();
  Serial.begin(BAUD_RATE);
  Serial.println("Hello World!"); // serial monitor test.
  serv.begin();
  imu_sensor.begin();
  gps_sensor.begin();
  barometer_sensor.begin();
  adc_sensor.begin();
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
    gps_sensor.update(); // Need to be updated every loop.
    if (current_time - last_update_time >= (1000 / SAMPLING_RATE)) {
      last_update_time = current_time;
      imu_sensor.update();
      barometer_sensor.update();
      adc_sensor.update();

      /* Logging sensor data to Serial. */
      Serial.print("Time: ");
      Serial.println(current_time);
      Serial.println(rtc.getTimeData());
      printSensorDataToSerial(imu_sensor);
      printSensorDataToSerial(gps_sensor);
      printSensorDataToSerial(barometer_sensor);
      printSensorDataToSerial(adc_sensor);
      Serial.println("");

      /* Writing sensor data to SD card. */
      sd_manager.write(rtc.getTimeData());
      sd_manager.write(imu_sensor.toString());
      sd_manager.write(gps_sensor.toString());
      sd_manager.write(barometer_sensor.toString());
      sd_manager.write(adc_sensor.toString());
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
