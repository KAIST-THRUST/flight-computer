#ifndef LOGGER_H
#define LOGGER_H

#include "Navigation.h"
#include "SensorSet.h"
#include "config.h"

enum class LogCategory { INFO, DATA, ERROR, DEBUG };

class LogFormatter {
public:
  virtual const char *format(LogCategory category,
                             const String &message) = 0; // 100 char max
  virtual const char *format(SensorDataCollection &data) = 0;

protected:
  char buffer[500];
};

class DefaultLogFormatter : public LogFormatter {
public:
  const char *format(LogCategory category,
                     const String &message) override {
    const char *category_str = nullptr;
    switch (category) {
    case LogCategory::INFO:
      category_str = "INFO";
      break;
    case LogCategory::DATA:
      category_str = "DATA";
      break;
    case LogCategory::ERROR:
      category_str = "ERROR";
      break;
    case LogCategory::DEBUG:
      category_str = "DEBUG";
      break;
    }
    sprintf(buffer, "[%07lu] [%s] %s", millis(), category_str,
            message.c_str());
    return buffer;
  }

  const char *format(SensorDataCollection &data) {
    sprintf(buffer,
            "[%07lu] [DATA] [GPS] Lat: %.7f, Long: %.7f, Alt: %.7f\n"
            "[%07lu] [DATA] [IMU] Acc: (%.7f, %.7f, %.7f)\n"
            "[%07lu] [DATA]       Ang vel: (%.7f, %.7f, %.7f)\n"
            "[%07lu] [DATA]       Quat: (%.7f, %.7f, %.7f, %.7f)\n"
            "[%07lu] [DATA] [Bar] Pres: %.7f, Temp: %.7f\n"
            "[%07lu] [DATA] [ADC] Volt: %.7f, Pres: %.7f",
            data.current_time, data.gps_data[GPSSensor::LATITUDE],
            data.gps_data[GPSSensor::LONGITUDE],
            data.gps_data[GPSSensor::ALTITUDE], data.current_time,
            data.imu_data[IMUSensor::ACC_X],
            data.imu_data[IMUSensor::ACC_Y],
            data.imu_data[IMUSensor::ACC_Z], data.current_time,
            data.imu_data[IMUSensor::ANG_VEL_X],
            data.imu_data[IMUSensor::ANG_VEL_Y],
            data.imu_data[IMUSensor::ANG_VEL_Z], data.current_time,
            data.imu_data[IMUSensor::QUAT_X],
            data.imu_data[IMUSensor::QUAT_Y],
            data.imu_data[IMUSensor::QUAT_Z],
            data.imu_data[IMUSensor::QUAT_W], data.current_time,
            data.barometer_data[BarometerSensor::PRESSURE],
            data.barometer_data[BarometerSensor::TEMPERATURE],
            data.current_time, data.adc_data[ADCSensor::VOLTAGE],
            data.adc_data[ADCSensor::PRESSURE]);
    return buffer;
  }

  const char *format(NavigationData &data) {
    sprintf(buffer,
            "[%07lu] [DATA] [NAV] pos: (%.7f, %.7f, %.7f)\n"
            "[%07lu] [DATA]       vel: (%.7f, %.7f, %.7f)\n"
            "[%07lu] [DATA]       max_alt: %.7f",
            data.current_time, data.pos_ENU[0], data.pos_ENU[1],
            data.pos_ENU[2], data.current_time, data.vel_ENU[0],
            data.vel_ENU[1], data.vel_ENU[2], data.current_time,
            data.max_altitude);
    return buffer;
  }
};

// Combined LogDevice interface
// class LogDevice {
// public:
//   LogDevice(LogFormatter *fmt) : formatter(fmt) {}
//   virtual void write(LogCategory category, const String &message) =
//   0; virtual void writeRaw(const uint8_t *data,
//                         size_t length) = 0; // New method for raw
//                         data
//   virtual bool available() { return false; }
//   virtual int read() {
//     return -1;
//   } // Changed to return int for compatibility with Stream
//   virtual size_t readBytes(uint8_t *buffer, size_t length) {
//     return 0;
//   } // New method for reading raw data
//   void setFormatter(LogFormatter *fmt) { formatter = fmt; }

// protected:
//   LogFormatter *formatter;
// };

#endif
