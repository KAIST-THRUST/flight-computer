#include "SDManager.h"

SDManager::SDManager() : file_opened(false), file_name(FILE_NAME) {}

void SDManager::begin(String file_name) {
  this->file_name = file_name;
  if (!SD.begin(BUILTIN_SDCARD)) {
    printErrorMessageToSerial("SD card initialization failed!");
    return;
  }
  open();
  file_opened = true;
}

void SDManager::write(const String &data) {
  /* If file is not opened, open it. */
  open();

  file.println(data); // Write data to file.

  /* Close and reopen file every 60 seconds. */
  if (file_opened && timer > 60 * 1000) {
    close();
    timer = timer - 60 * 1000;

    /* Reopen file. */
    open();
  }
}

void SDManager::open() {
  if (!file_opened) {
    file = SD.open(file_name.c_str(), FILE_WRITE);
    if (!file) {
      printErrorMessageToSerial("File open failed!");
      return;
    }
    file_opened = true;
  }
}

void SDManager::close() {
  if (file_opened) {
    file.close();
    file_opened = false;
  }
}
