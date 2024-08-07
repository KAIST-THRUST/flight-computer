#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "DefaultLogFormatter.h"
#include "LogDevice.h"
#include "Navigation.h"
#include "NonBlockingServo.h"
#include "RealTimeClock.h"
#include "SdDevice.h"
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

  void updateSd();

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
  SerialDevice hc12;            // Serial device for writing data.
  SdDevice sd_device;           // SD card device for writing data.
  byte hc12_buffer[110];        // Buffer for HC12 communication device.
  elapsedMillis since_transmit; // Timer for HC12 communication device.

  /* Time related variables. */
  elapsedMillis since_boot; // Time elapsed since booting started.
  elapsedMillis since_fix;  // Time elapsed since initial fix.
  elapsedMillis since_burn; // Time elapsed since burn started.

  /* Helper functions for state transition condition check. */
  bool shouldChangeToBurn();
  bool shouldChangeToCoast();
  bool shouldEject();
  bool shouldChangeToLanded();

  /* Helper functions. */
  void initializeNavigation();    // Initialize navigation data.
  void updateNavigation();        // Update navigation data.
  void updateSensorData();        // Update sensor data.
  void updateAverageSensorData(); // Update average sensor data.
  void sendDataToHc12();          // Send data to HC12.
};

#endif
