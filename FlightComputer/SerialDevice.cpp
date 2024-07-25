// #include "SerialDevice.h"

// SerialDevice::SerialDevice(HardwareSerial &hs, LogFormatter *fmt)
//     : LogDevice(fmt), serial(hs) {}

// void SerialDevice::begin() { serial.begin(HC12_BAUD_RATE); }

// void SerialDevice::write(LogCategory category, const String &message) {
//   serial.print(formatter->format(category, message));
// }

// void SerialDevice::write(SensorDataCollection &data) {
//   serial.print(formatter->format(data));
// }

// void SerialDevice::write(NavigationData &data) {
//   serial.print(formatter->format(data));
// }

// void SerialDevice::writeRaw(const byte *data, size_t length) {
//   serial.write(data, length);
// }

// bool SerialDevice::available() { return serial.available(); }

// size_t SerialDevice::read(byte *buffer, size_t length) {
//   return serial.readBytes(buffer, length);
// }
