#include "GPSSensor.h"

GPSSensor::GPSSensor() : GPS(&GPSSERIAL) {
  sensorData.data_count = DATA_COUNT;
}

void GPSSensor::begin() {
  GPS.begin(9600);

  /* Turn on RMC and GGA including altitude. */
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  /* 1Hz update rate. */
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
}

void GPSSensor::update() {
  /* GPS update rate should not exceed 1Hz. */
  static unsigned long last_update_time = 0;
  unsigned long current_time = millis();

  if (current_time - last_update_time < (1000 / GPS_SAMPLING_RATE))
    return;

  // char c = GPS.read(); // For debugging purposes.

  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))
      /* If parse fails, return. */
      return;
  }

  if (GPS.fix) {
    sensorData.values[LATITUDE] = GPS.latitude;
    sensorData.values[LONGITUDE] = GPS.longitude;
    sensorData.values[ALTITUDE] = GPS.altitude;
  }

  last_update_time = current_time;
}

String GPSSensor::toString() const {
  return "Latitude: " + String(sensorData.values[LATITUDE]) +
         ", Longitude: " + String(sensorData.values[LONGITUDE]) +
         ", Altitude: " + String(sensorData.values[ALTITUDE]);
}

float GPSSensor::getLatitude() const {
  return sensorData.values[LATITUDE];
}

float GPSSensor::getLongitude() const {
  return sensorData.values[LONGITUDE];
}

float GPSSensor::getAltitude() const {
  return sensorData.values[ALTITUDE];
}
