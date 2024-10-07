/**
 * @file RealTimeClock.h
 * @brief A real time clock class that reads data from Teensy's built in
 * clock.
 */

#ifndef REALTIMECLOCK_H
#define REALTIMECLOCK_H

#include "config.h"
#include <TimeLib.h>

struct TimeData {
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
  uint32_t millis;

  void toString(char *buffer) const {
    sprintf(buffer, "%d-%02d-%02d %02d-%02d-%02d", year, month, day,
            hour, minute, second);
  }
};

class RealTimeClock {
public:
  RealTimeClock();
  void begin();
  const TimeData &getTimeData();

private:
  TimeData time_data;
};

#endif
