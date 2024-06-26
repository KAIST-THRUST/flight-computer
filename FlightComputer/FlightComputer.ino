/**
 * @file FlightComputer.ino
 * @brief Main sketch file for execution of the flight computer.
 */

#include "GPSSensor.h"
#include "IMUSensor.h"
#include "NonBlockingServo.h"
#include "SensorSet.h"
#include "config.h"
#include "util.h"
#include "SDManager.h"

/* Sensors to receive data. */
static NonBlockingServo serv;
static IMUSensor imu_sensor;
static GPSSensor gps_sensor;

/* Time variables for non-blocking delay(). */
static unsigned long last_update_time = 0;
static unsigned long current_time;

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Hello World!"); // serial monitor test.
  serv.begin();
  imu_sensor.begin();
  gps_sensor.begin();
}

void loop() {
  /*-----------------------------------------------------------------*/
  /* Servo part. */
  updateServoFromSerial(serv);
  /*-----------------------------------------------------------------*/

  /*-----------------------------------------------------------------*/
  /* Sensor update part. */
  current_time = millis();
  if (current_time - last_update_time > (1000 / SAMPLING_RATE)) {
    last_update_time = current_time;
    imu_sensor.update();
    gps_sensor.update();
    printSensorDataToSerial(imu_sensor);
    printSensorDataToSerial(gps_sensor);
  }
  /*-----------------------------------------------------------------*/
}
