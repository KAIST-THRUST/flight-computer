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
  char c = GPS.read();

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
