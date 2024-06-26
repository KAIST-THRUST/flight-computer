/**
 * @file util.h
 * @brief Utility functions for the project.
 */

#ifndef UTIL_H
#define UTIL_H

#include "NonBlockingServo.h"
#include "config.h"

/* Update servo motor degree from serial. */
void updateServoFromSerial(NonBlockingServo &serv);

/* Print error message s to serial monitor. */
void printErrorMessageToSerial(String s);

#endif
