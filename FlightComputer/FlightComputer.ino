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
static unsigned long last_update_time = 0;
static unsigned long current_time;
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
  // switch (tu_1_current_state) {
  // case state::ST_STAND_BY:
  //   // Stand by state.
  //   break;

  // case state::ST_BURN:
  //   // Burn state.
  //   break;

  // case state::ST_COAST:
  //   // Coast state.
  //   break;

  // case state::ST_DESCENT:
  //   // Descent state.
  //   break;

  // case state::ST_LANDED:
  //   // Landed state.
  //   break;
  // }

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
    Serial.println(rtc.getTimeData());
    printSensorDataToSerial(imu_sensor);
    printSensorDataToSerial(gps_sensor);
    printSensorDataToSerial(barometer_sensor);
    printSensorDataToSerial(adc_sensor);
    Serial.println("------------------------------------------------");
  }
  /*-----------------------------------------------------------------*/
}
