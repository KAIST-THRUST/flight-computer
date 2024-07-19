#ifndef DEFAULTLOGFORMATTER_H
#define DEFAULTLOGFORMATTER_H

#include "Logger.h"
#include "SensorSet.h"
#include "config.h"

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
            "[%07lu] [DATA] [GPS] Lat: %.7f, Long: %.7f, Alt: %.7f, "
            "Geo: %.7f\n"
            "[%07lu] [DATA] [GPS] Lat_LS: %.7f, Long_LS: %.7f, Alt_LS: "
            "%.7f, Geo_LS: %.7f\n"
            "[%07lu] [DATA] [IMU] Acc: (%.7f, %.7f, %.7f)\n"
            "[%07lu] [DATA]       Ang vel: (%.7f, %.7f, %.7f)\n"
            "[%07lu] [DATA]       Quat: (%.7f, %.7f, %.7f, %.7f)\n"
            "[%07lu] [DATA] [Bar] Pres: %.7f, Temp: %.7f\n"
            "[%07lu] [DATA] [Bar] Pres_AVG: %.7f, Temp_AVG: %.7f\n"
            "[%07lu] [DATA] [ADC] Volt: %.7f, Pres: %.7f",
            data.current_time, data.gps_data[GPSSensor::LATITUDE],
            data.gps_data[GPSSensor::LONGITUDE],
            data.gps_data[GPSSensor::ALTITUDE],
            data.gps_data[GPSSensor::GEOID_HEIGHT], data.current_time,
            data.gps_data[GPSSensor::LATITUDE_LS],
            data.gps_data[GPSSensor::LONGITUDE_LS],
            data.gps_data[GPSSensor::ALTITUDE_LS],
            data.gps_data[GPSSensor::GEOID_HEIGHT_LS],
            data.current_time, data.imu_data[IMUSensor::ACC_X],
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
            data.current_time,
            data.barometer_data[BarometerSensor::PRESSURE_AVG],
            data.barometer_data[BarometerSensor::TEMPERATURE_AVG],
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

#endif
