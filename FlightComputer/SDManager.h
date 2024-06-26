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
  void begin();                   // Initialize SD card.
  void write(const String &data); // Write data to SD card.

private:
  File file;
};

#endif
