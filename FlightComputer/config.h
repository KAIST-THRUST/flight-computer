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

/* Barometer sensor(BMP280) configurations. */
#define BAROMETER_I2C_ADDRESS 0x76 // I2C address of BMP280 sensor.
#define BAROMETER_WIRE Wire1       // Wire name connected to BMP280.

/* IMU sensor(BNO055) configurations. */
#define IMU_I2C_ADDRESS 0x28  // I2C address of BNO055 sensor.
#define IMU_WIRE Wire         // Wire name connected to BNO055.
#define SEA_LEVEL_HPA 1013.25 // Sea level pressure in hPa.

/* GPS sensor configurations. */
#define GPS_SERIAL Serial2  // Serial name connected to GPS.
#define GPS_SAMPLING_RATE 1 // GPS sampling rate, in Hz.

/* SD card configurations. */
#define FILE_NAME "data.txt" // File name to store sensor data.

/*-------------------------------------------------------------------*/

#endif
