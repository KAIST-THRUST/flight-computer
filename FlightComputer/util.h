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

#endif
