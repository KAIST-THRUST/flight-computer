/**
 * @file LogDevice.h
 * @brief A log device class and formatter class that logs data.
 */
#ifndef LOGDEVICE_H
#define LOGDEVICE_H

#include "Navigation.h"
#include "SensorSet.h"
#include "config.h"

enum class LogCategory : byte { INFO, DATA, WARNING, ERROR, DEBUG };

/**
 * @brief A log formatter class that formats log data.
 * @note This class can write max 1000 characters at once.
 */
class LogFormatter {
public:
  virtual const char *format(LogCategory category,
                             const char *message) = 0;
  virtual const char *format(SensorDataCollection &data) = 0;
  virtual const char *format(NavigationData &data) = 0;

protected:
  char buffer[1000]; // Buffer for formatted log data.
};

/**
 * @brief A log device class that logs data.
 */
class LogDevice {
public:
  LogDevice(LogFormatter *fmt) : formatter(fmt) {}

  virtual void begin() = 0;

  /* Write log data. '\n' is added at the end of message. */
  virtual void write(LogCategory category, const char *message) = 0;
  virtual void write(SensorDataCollection &data) = 0;
  virtual void write(NavigationData &data) = 0;

  /* Write raw data. */
  virtual void writeRaw(const byte *data, size_t length) = 0;

  /* Check if data is available to read. */
  virtual bool available() { return false; }

  /* Check if device is connected. */
  virtual bool isConnected() = 0;

  /* Read data. */
  virtual size_t read(byte *buffer, size_t length) { return 0; }

  /* Set formatter for this log device. */
  void setFormatter(LogFormatter *fmt) { formatter = fmt; }

protected:
  LogFormatter *formatter; // Formatter that log device uses.
};

#endif
