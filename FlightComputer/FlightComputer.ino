/**
 * @file FlightComputer.ino
 * @brief Main sketch file for execution of the flight computer.
 */

#include "NonBlockingServo.h"
#include "SDManager.h"
#include "SensorSet.h"
#include "config.h"
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
static unsigned long last_update_time = 0;
static unsigned long current_time;

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Hello World!"); // serial monitor test.
  serv.begin();
  imu_sensor.begin();
  gps_sensor.begin();
  barometer_sensor.begin();
  adc_sensor.begin();
  delay(1000); // Wait for sensors to initialize.
  sd_manager.begin();
  sd_manager.write("Test string.");
}

void loop() {
  /*-----------------------------------------------------------------*/
  /* Servo part. */
  updateServoFromSerial(serv);
  /*-----------------------------------------------------------------*/

  /*-----------------------------------------------------------------*/
  /* Sensor update part. */
  current_time = millis();
  gps_sensor.update();

  if (current_time - last_update_time >= (1000 / SAMPLING_RATE)) {
    last_update_time = current_time;
    imu_sensor.update();
    barometer_sensor.update();
    adc_sensor.update();
    Serial.println("------------------------------------------------");
    Serial.print("Time: ");
    Serial.println(current_time);
    printSensorDataToSerial(imu_sensor);
    printSensorDataToSerial(gps_sensor);
    printSensorDataToSerial(barometer_sensor);
    printSensorDataToSerial(adc_sensor);
    Serial.println("------------------------------------------------");
  }
  /*-----------------------------------------------------------------*/
}
