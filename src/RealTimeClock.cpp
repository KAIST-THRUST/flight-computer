#include "RealTimeClock.h"

// Helper function for getting the time from the Teensy clock.
time_t getTeensyTime() { return Teensy3Clock.get(); }

RealTimeClock::RealTimeClock() { time_data = {0, 0, 0, 0, 0, 0, 0}; }

void RealTimeClock::begin() { setSyncProvider(getTeensyTime); }

const TimeData &RealTimeClock::getTimeData() {
  if (timeStatus() == timeNotSet) {
    Serial.println("Error: Unable to sync with the RTC");
  }

  /* Update the time_data variable. */
  time_data.year = year();
  time_data.month = month();
  time_data.day = day();
  time_data.hour = hour();
  time_data.minute = minute();
  time_data.second = second();
  time_data.millis = millis();
  return time_data;
}
