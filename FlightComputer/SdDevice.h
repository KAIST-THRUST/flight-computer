/**
 * @file SdDevice.h
 * @brief A SD card device class that writes data to SD card.
 */

#ifndef SDDEVICE_H
#define SDDEVICE_H

#include "LogDevice.h"
#include "config.h"
#include <RingBuf.h>
#include <SdFat.h>

class SdDevice : public LogDevice {
public:
  SdDevice(LogFormatter *fmt) : LogDevice(fmt), file_name(FILE_NAME) {}

  void begin() override {}
  void write(LogCategory category, const char *message) override;
  void write(SensorDataCollection &data) override;
  void write(NavigationData &data) override;

  void writeRaw(const byte *data, size_t length) override {}

  bool available() override {}

  void setFileName(const char *name) { strcpy(file_name, name); }

private:
  SdFs sd;
  FsFile file;
  RingBuf<FsFile, 512> file_buf;
  char file_name[50];
};

#endif
