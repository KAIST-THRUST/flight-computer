/**
 * @file configuration.h
 * @brief File containing configuration information of our project.
 */

#ifndef CONFIG_H
#define CONFIG_H

/*-------------------------------------------------------------------*/
/* Define or modify the configurations here if you want. */

#define BAUD_RATE 115200    // Baud rate of the main serial port.
#define GPSSERIAL Serial2   // Serial name connected to GPS.
#define MAX_DATA_COUNT 10   // Maximum number of sensor output data.
#define SERVO_PIN 2         // Servo motor pin number.
#define SAMPLING_RATE 100   // Sampling rate, in Hz.
#define GPS_SAMPLING_RATE 1 // GPS sampling rate, in Hz.
#define ROTATION_SPEED 0.2  // Rotation speed in (degree/ms).

/*-------------------------------------------------------------------*/

#endif
