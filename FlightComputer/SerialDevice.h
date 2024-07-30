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
  SerialDevice(HardwareSerial &hs, LogFormatter *fmt)
      : LogDevice(fmt), serial(hs) {}

  void begin() override { serial.begin(HC12_BAUD_RATE); }

  void write(LogCategory category, const char *message) override {
    serial.print(formatter->format(category, message));
  }

  void write(SensorDataCollection &data) override {
    serial.print(formatter->format(data));
  }

  void write(NavigationData &data) override {
    serial.print(formatter->format(data));
  }

  void writeRaw(const byte *data, size_t length) override {
    serial.write(data, length);
    serial.flush();
  }

  bool available() override { return serial.available(); }

  bool isConnected() override { return serial; }

  size_t read(byte *buffer, size_t length) override {
    return serial.readBytes(buffer, length);
  }

private:
  HardwareSerial &serial;
};

class UsbSerialDevice : public LogDevice {
public:
  UsbSerialDevice(usb_serial_class &serial, LogFormatter *fmt)
      : LogDevice(fmt), serial(serial) {}
  void begin() override { serial.begin(HC12_BAUD_RATE); }

  void write(LogCategory category, const char *message) override {
    serial.print(formatter->format(category, message));
  }

  void write(SensorDataCollection &data) override {
    serial.print(formatter->format(data));
  }

  void write(NavigationData &data) override {
    serial.print(formatter->format(data));
  }

  void writeRaw(const byte *data, size_t length) override {
    serial.write(data, length);
  }

  bool available() override { return serial.available(); }

  bool isConnected() override { return serial; }

  size_t read(byte *buffer, size_t length) override {
    return serial.readBytes((char *)buffer, length);
  }

private:
  usb_serial_class &serial;
};

#endif
