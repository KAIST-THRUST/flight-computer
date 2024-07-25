/**
 * @file SdDevice.h
 * @brief A SD card device class that writes data to SD card.
 */

#ifndef SDDEVICE_H
#define SDDEVICE_H

#include "LogDevice.h"
#include "config.h"
#include <SD.h>
#include <SPI.h>

class SdDevice : public LogDevice {
public:
  SdDevice(LogFormatter *fmt) : LogDevice(fmt), file_name(FILE_NAME) {}

  void begin() override {
    if (!SD.begin(BUILTIN_SDCARD)) {
      return;
    }
  }
  void write(LogCategory category, const char* message) override;
  void write(SensorDataCollection &data) override;
  void write(NavigationData &data) override;
  
  void writeRaw(const byte *data, size_t length) override {
    file.write(data, length);
  }

  bool available() override {
    return file.available();
  }

  void setFileName(const char *name) {
    strcpy(file_name, name);
  }

private:
  File file;
  char file_name[50];
};

#endif
