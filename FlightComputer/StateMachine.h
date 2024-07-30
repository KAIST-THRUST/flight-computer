#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "DefaultLogFormatter.h"
#include "LogDevice.h"
#include "Navigation.h"
#include "NonBlockingServo.h"
#include "RealTimeClock.h"
#include "SDManager.h"
#include "SensorSet.h"
#include "SerialDevice.h"
#include "config.h"

class StateMachine {
public:
  StateMachine();
  void begin();

  /* State functions. */
  void boot();
  void standBy();
  void burn();
  void coast();
  void descend();
  void landed();

private:
  /* Sensor related objects. */
  SensorDataCollection sensor_data_collection; // Sensor data stored.
  SensorSet sensor_set; // Sensor set for reading data from sensors.
  AverageSensorValue gps_latitude_ls;
  AverageSensorValue gps_longitude_ls;
  AverageSensorValue gps_altitude_ls;
  AverageSensorValue gps_geoid_height_ls;
  AverageSensorValue bar_pressure_avg;
  AverageSensorValue bar_temperature_avg;

  /* Navigation related objects. */
  Navigation navigation; // Navigation object for navigation data.
  NavigationData navigation_data; // Navigation data stored.

  /* Other objects. */
  NonBlockingServo servo; // Servo motor for ejection mechanism.
  RealTimeClock rtc;      // Real time clock for time data.

  /* Logger. */
  DefaultLogFormatter log_formatter; // Log formatter for logging data.
  SDManager sd_manager;     // SD card manager for writing data.
  SerialDevice hc12;        // Serial device for writing data.
  byte hc12_buffer[110];    // Buffer for HC12 communication device.
  elapsedMillis hc12_timer; // Timer for HC12 communication device.

  /* Time related variables. */
  elapsedMillis since_boot; // Time elapsed since booting started.
  elapsedMillis since_fix;  // Time elapsed since initial fix.
  elapsedMillis since_burn; // Time elapsed since burn started.

  /* Helper functions for state transition condition check. */
  bool shouldEject();

  /* Helper functions. */
  void initializeNavigation();    // Initialize navigation data.
  void updateNavigation();        // Update navigation data.
  void updateSensorData();        // Update sensor data.
  void updateAverageSensorData(); // Update average sensor data.
};

#endif
