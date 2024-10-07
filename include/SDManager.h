/**
 * @file SDManager.h
 * @brief A SD card manager class that writes data to Teensy's built in
 * SD card.
 */

#ifndef SDMANAGER_H
#define SDMANAGER_H

#include "config.h"
#include "util.h"
#include <SD.h>
#include <SPI.h>

class SDManager {
public:
  SDManager();
  bool begin(String file_name = FILE_NAME); // Initialize SD card.
  void write(const String &data);           // Write data to SD card.
  void open();                              // Open the file.
  void close();                             // Close the file.

private:
  File file;
  bool file_opened;
  elapsedMillis timer;
  String file_name;
  bool is_available;
};

#endif
