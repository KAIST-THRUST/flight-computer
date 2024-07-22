/**
 * @file SerialDevice.h
 * @brief A serial device class that writes data to serial port.
 */

#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include "LogDevice.h"
#include "config.h"

class SerialDevice : public LogDevice {
public:
  SerialDevice(HardwareSerial &hs, LogFormatter *fmt);
  void begin() override;
  void write(LogCategory category, const String &message) override;
  void write(SensorDataCollection &data) override;
  void write(NavigationData &data) override;
  void writeRaw(const byte *data, size_t length) override;
  bool available() override;
  size_t read(byte *buffer, size_t length) override;

private:
  HardwareSerial &serial;
};

#endif
