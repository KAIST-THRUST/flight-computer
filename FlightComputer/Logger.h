#ifndef LOGGER_H
#define LOGGER_H

#include "Navigation.h"
#include "SensorSet.h"
#include "config.h"

enum class LogCategory { INFO, DATA, ERROR, DEBUG };

class LogFormatter {
public:
  virtual const char *format(LogCategory category,
                             const String &message) = 0; // 100 char max
  virtual const char *format(SensorDataCollection &data) = 0;

protected:
  char buffer[1000];
};

// Combined LogDevice interface
class LogDevice {
public:
  LogDevice(LogFormatter *fmt) : formatter(fmt) {}
  virtual void write(LogCategory category, const String &message) = 0;
  virtual void writeRaw(const uint8_t *data,
                        size_t length) = 0; // New method for raw data
  virtual bool available() { return false; }
  virtual int read() {
    return -1;
  } // Changed to return int for compatibility with Stream
  virtual size_t readBytes(uint8_t *buffer, size_t length) {
    return 0;
  } // New method for reading raw data
  void setFormatter(LogFormatter *fmt) { formatter = fmt; }

protected:
  LogFormatter *formatter;
};

#endif
