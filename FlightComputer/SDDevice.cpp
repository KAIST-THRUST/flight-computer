#include "SdDevice.h"

void SdDevice::begin() {
  // Wait for SD card to be inserted.
  while (!sd.begin(SdioConfig(FIFO_SDIO))) {
    delay(1000);
  }

  // Open file for writing.
  if (!file.open(file_name, O_RDWR | O_CREAT | O_TRUNC)) {
#ifdef FC_DEBUG_ENABLED
    Serial.println("Failed to open file for writing.");
#endif
    return;
  }

  // Preallocate file.
  if (!file.preAllocate(LOG_FILE_SIZE)) {
#ifdef FC_DEBUG_ENABLED
    Serial.println("Failed to preallocate file.");
#endif
    return;
  }

  file_buf.begin(&file);
}

void SdDevice::write(LogCategory category, const char *message) {
  file_buf.write(formatter->format(category, message));
  if (file_buf.getWriteError()) {
#ifdef FC_DEBUG_ENABLED
    Serial.println("Write error.");
#endif
    return;
  }
}

void SdDevice::write(SensorDataCollection &data) {
  file_buf.print(formatter->format(data));
  if (file_buf.getWriteError()) {
#ifdef FC_DEBUG_ENABLED
    Serial.println("Write error.");
#endif
    return;
  }
}

void SdDevice::write(NavigationData &data) {
  file_buf.print(formatter->format(data));
  if (file_buf.getWriteError()) {
#ifdef FC_DEBUG_ENABLED
    Serial.println("Write error.");
#endif
    return;
  }
}

void SdDevice::update() {
  size_t n = file_buf.bytesUsed();
  if ((n + file.curPosition()) > (LOG_FILE_SIZE - 2 * 150)) {
#ifdef FC_DEBUG_ENABLED
    Serial.println("File full.");
#endif
    return;
  }

  if (n >= 1024 && !file.isBusy()) {
    if (1024 != file_buf.writeOut(1024)) {
#ifdef FC_DEBUG_ENABLED
      Serial.println("Writeout failed.");
#endif
      return;
    }
  }
}
