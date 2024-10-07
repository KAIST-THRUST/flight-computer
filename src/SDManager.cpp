#include "SDManager.h"

SDManager::SDManager() : file_opened(false), file_name(FILE_NAME) {}

bool SDManager::begin(String file_name) {
  this->file_name = file_name;
  if (!SD.begin(BUILTIN_SDCARD)) {
    // printErrorMessageToSerial("SD card initialization failed!");
    is_available = false;
    return false;
  }
  is_available = true;
  open();
  close();
  return true;
}

void SDManager::write(const String &data) {
  /* If file is not opened, open it. */
  open();

  file.print(data); // Write data to file.

  /* Close and reopen file every 10 seconds. */
  if (file_opened && timer > 10 * 1000) {
    close();
    timer = timer - 10 * 1000;

    /* Reopen file. */
    open();
  }
}

void SDManager::open() {
  if (!file_opened) {
    file = SD.open(file_name.c_str(), FILE_WRITE);
    if (!file) {
#ifdef FC_DEBUG_ENABLED
      printErrorMessageToSerial("File open failed!");
#endif
      return;
    }
    file_opened = true;
  }
}

void SDManager::close() {
  file.println("File closed.");
  if (file_opened) {
    file.close();
    file_opened = false;
  }
}
