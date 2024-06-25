/**
 * @file FlightComputer.ino
 * @brief Main sketch file for execution of the flight computer.
 */

#include "config.h"
#include "util.h"
#include "NonBlockingServo.h"

static NonBlockingServo serv;

void setup() {
  Serial.begin(9600);
  Serial.println("Hello World!"); // serial monitor test.
  serv.begin();
}

void loop() {
  /*----------------------------------------------------------------*/
  /* Servo part. */
  updateServoFromSerial(serv);
  /*----------------------------------------------------------------*/
}
