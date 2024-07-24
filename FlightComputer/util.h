/**
 * @file util.h
 * @brief Utility functions for the project.
 */

#ifndef UTIL_H
#define UTIL_H

#include "NonBlockingServo.h"
#include "Sensor.h"
#include "config.h"
#include <math.h>

#define S_TO_MS(x) ((x) * 1000)        // Convert second to millisecond.
#define MIN_TO_MS(x) ((x) * 60 * 1000) // Convert minute to millisecond.

/* Update servo motor degree from serial. */
void updateServoFromSerial(NonBlockingServo &serv);

/* Print error message s to serial monitor. */
void printErrorMessageToSerial(String s);

/* Print sensor data information to serial monitor. */
void printSensorDataToSerial(const Sensor &sensor);

/* Vector calculation related utility functions. */
float dotProduct(float v_A[3], float v_B[3]);
float calcNorm(float v[3]);
void crossProduct(float v_A[3], float v_B[3], float res[3]);
void matrixVecMult(float T[3][3], float b[3], float res[3]);
void matrixTranspose(float A[3][3], float res[3][3]);

/* 3D Attitude and frame conversion related utility functions. */
void quat_to_DCM(float quat[4], float dcm[3][3]);
void lla_to_ECEF(float lat_deg, float lon_deg, float alt_wgs84,
                 float r_ECEF_m[3]);
void get_dcm_ECEF_to_ENU(float lat_deg, float lon_deg,
                         float dcm_ECEF_to_ENU[3][3]);

#endif
