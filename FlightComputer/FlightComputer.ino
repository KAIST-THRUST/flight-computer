/**
 * @file FlightComputer.ino
 * @brief Main sketch file for execution of the flight computer.
 */

#include "IMUSensor.h"
#include "NonBlockingServo.h"
#include "config.h"
#include "util.h"

static NonBlockingServo serv;
static IMUSensor imu_sensor;

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Hello World!"); // serial monitor test.
  serv.begin();
}

void loop() {
  /*-----------------------------------------------------------------*/
  /* Servo part. */
  updateServoFromSerial(serv);
  /*-----------------------------------------------------------------*/
}
