#include "SDManager.h"

SDManager::SDManager() {}

void SDManager::begin() {
  if (!SD.begin(BUILTIN_SDCARD)) {
    printErrorMessageToSerial("SD card initialization failed!");
    return;
  }
}

void SDManager::write(const String &data) {
  file = SD.open(FILE_NAME, FILE_WRITE);
  if (!file) {
    printErrorMessageToSerial("File open failed!");
    return;
  }
  file.println(data);
  file.close();
}
