/**
 * @file config.h
 * @brief File containing configuration information of our project.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/* Define the state of the rocket. */
enum RocketState : byte {
  ST_BOOT,
  ST_STAND_BY,
  ST_BURN,
  ST_COAST,
  ST_DESCENT,
  ST_LANDED
};

extern enum RocketState rocket_current_state;

/*-------------------------------------------------------------------*/
/* Define or modify the configurations here if you want. */
#define FC_DEBUG_ENABLED // Enable debug mode.
#define FC_TEST_ENABLED  // Enable test mode.

/* Basic configurations. */
#define BAUD_RATE 115200  // Baud rate of the main serial port.
#define MAX_DATA_COUNT 10 // Maximum number of sensor output data.
#define SAMPLING_RATE 100 // Base sampling rate, in Hz.
#define INITIAL_ROCKET_STATE RocketState::ST_BOOT // Initial state.
#define BOOT_TIME 30                              // Boot time, in sec.

/* Navigation configurations. */
// Gyro Low Pass Filter Config
#define T_S 0.01   // Nominal sampling rate of gyroscope, in sec
#define F_CUTOFF 5 // Cutoff frequency of low pass filter, in Hz

// Quadratic Filter Config
#define Q_ALPHA 0.3
#define Q_BETA 0.37
#define Q_ZETA 0.37
#define Q_BATCH_SIZE 10

// TU-1.f Vehicle Config
#define M_DRY 3.7 // Dry mass of the vehicle after burnout, in kg
#define C_D0 0.3  // Zero lift drag coefficient of the vehicle
#define S_REF 0.00849486653 // Ref area for aerodynamic coeff, in m^2
#define IMU_CG_DIST 0       // Distance from cg to imu, in m

/* Servo motor configurations. */
#define SERVO_PIN 2        // Servo motor pin number.
#define ROTATION_SPEED 0.2 // Rotation speed in (degree/ms).

/* Barometer sensor(BMP280) configurations. */
#define BAROMETER_I2C_ADDRESS 0x76 // I2C address of BMP280 sensor.
#define BAROMETER_WIRE Wire1       // Wire name connected to BMP280.

/* IMU sensor(BNO055) configurations. */
#define IMU_I2C_ADDRESS 0x28 // I2C address of BNO055 sensor.
#define IMU_WIRE Wire        // Wire name connected to BNO055.

/* GPS sensor configurations. */
#define GPS_SERIAL Serial2  // Serial name connected to GPS.
#define GPS_SAMPLING_RATE 1 // GPS sampling rate, in Hz.

/* ADC (ADS1125) configurations. */
#define ADC_I2C_ADDRESS 0x48 // I2C address of ADS1125.
#define ADC_WIRE Wire2       // Wire name connected to ADS1125.
#define ADC_PRESSURE_PIN 2   // Pressure sensor pin number.
#define ADC_VOLTAGE_PIN 3    // Voltage sensor pin number.
#define ADC_SAMPLING_RATE 10 // ADC sampling rate, in Hz.

/* SD card configurations. */
#define FILE_NAME "sensor_data.txt" // File name to store sensor data.
#define LOG_FILE_SIZE (150 * SAMPLING_RATE * 60 * 60) // File size.

/* HC12 configuration. */
#define HC12_SERIAL Serial1   // Serial name connected to HC12.
#define HC12_BAUD_RATE 9600   // Baud rate of HC12.
#define HC12_SAMPLING_RATE 10 // HC12 sampling rate, in Hz.

/*-------------------------------------------------------------------*/

#endif
