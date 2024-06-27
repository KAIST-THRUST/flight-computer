/**
 * @file configuration.h
 * @brief File containing configuration information of our project.
 */

#ifndef CONFIG_H
#define CONFIG_H

/*-------------------------------------------------------------------*/
/* Define or modify the configurations here if you want. */

/* Basic configurations. */
#define BAUD_RATE 115200  // Baud rate of the main serial port.
#define MAX_DATA_COUNT 10 // Maximum number of sensor output data.
#define N_SENSORS 6       // Number of sensors.
#define SAMPLING_RATE 100 // Base sampling rate, in Hz.

/* Servo motor configurations. */
#define SERVO_PIN 2        // Servo motor pin number.
#define ROTATION_SPEED 0.2 // Rotation speed in (degree/ms).

/* Pressure sensor(BMP280) configurations. */
#define PRESSURE_I2C_ADDRESS 0x76 // I2C address of BMP280 sensor.

/* GPS sensor configurations. */
#define GPSSERIAL Serial2   // Serial name connected to GPS.
#define GPS_SAMPLING_RATE 1 // GPS sampling rate, in Hz.

/* SD card configurations. */
#define FILE_NAME "data.txt" // File name to store sensor data.

/*-------------------------------------------------------------------*/

#endif
