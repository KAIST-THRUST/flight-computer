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

  delay(1000);

  GPSSERIAL.println(PMTK_Q_RELEASE);
}

void GPSSensor::update() {
  /* GPS update rate should not exceed 1Hz. */
  static unsigned long last_update_time = 0;
  unsigned long current_time = millis();

  if (current_time - last_update_time < (1000 / GPS_SAMPLING_RATE))
    return;

  last_update_time = current_time;

  char c = GPS.read(); // For debugging purposes.

  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c)
      Serial.print(c);

  if (GPS.newNMEAreceived()) {
    if (!GPS.parse(GPS.lastNMEA()))
      /* If parse fails, return. */
      return;
  }

  if (GPS.fix) {
    /* Direction of latitude and longitude in sign. */
    int lat_dir = (GPS.lat == 'N') ? 1 : -1;
    int lon_dir = (GPS.lon == 'E') ? 1 : -1;

    sensorData.values[LATITUDE] = GPS.latitude * lat_dir;
    sensorData.values[LONGITUDE] = GPS.longitude * lon_dir;
    sensorData.values[ALTITUDE] = GPS.altitude;
    sensorData.values[SPEED] = GPS.speed * 0.51444; // Convert to m/s.
  }
}

String GPSSensor::toString() const {
  return "Latitude: " + String(sensorData.values[LATITUDE]) +
         ", Longitude: " + String(sensorData.values[LONGITUDE]) +
         ", Altitude: " + String(sensorData.values[ALTITUDE]) +
         ", Speed: " + String(sensorData.values[SPEED]);
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
