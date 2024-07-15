#include "GPSSensor.h"

GPSSensor::GPSSensor() : GPS(&GPS_SERIAL) {
  sensor_data.data_count = DATA_COUNT;
}

void GPSSensor::begin() {
  GPS.begin(9600);

  /* Turn on RMC and GGA including altitude. */
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  /* 1Hz update rate. */
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);

  GPS_SERIAL.println(PMTK_Q_RELEASE);
}

void GPSSensor::update() {
  /* GPS update rate should not exceed 1Hz. */
  static unsigned long last_update_time = 0;
  unsigned long current_time = millis();

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

  if (current_time - last_update_time < (1000 / GPS_SAMPLING_RATE))
    return;
  last_update_time = current_time;

  if (GPS.fix) {
    /* Direction of latitude and longitude in sign. */
    int lat_dir = (GPS.lat == 'N') ? 1 : -1;
    int lon_dir = (GPS.lon == 'E') ? 1 : -1;

    sensor_data.values[LATITUDE] = GPS.latitude * lat_dir;
    sensor_data.values[LONGITUDE] = GPS.longitude * lon_dir;
    sensor_data.values[ALTITUDE] = GPS.altitude;
    sensor_data.values[SPEED] = GPS.speed * 0.51444; // Convert to m/s.
    if (millis() <= 60 * 1000) {
      altitude_avg.addValue(sensor_data.values[ALTITUDE]);
      sensor_data.values[ALTITUDE_AVG] = altitude_avg.getAverage();
    }
  }
}

String GPSSensor::toString() const {
  return "[GPS] Latitude: " + String(sensor_data.values[LATITUDE], 7) +
         ", Longitude: " + String(sensor_data.values[LONGITUDE], 7) +
         ", Altitude: " + String(sensor_data.values[ALTITUDE], 7) +
         ", Average Altitude: " +
         String(sensor_data.values[ALTITUDE_AVG], 7) +
         ", Speed: " + String(sensor_data.values[SPEED], 7);
}

float GPSSensor::getLatitude() const {
  return sensor_data.values[LATITUDE];
}

float GPSSensor::getLongitude() const {
  return sensor_data.values[LONGITUDE];
}

float GPSSensor::getAltitude() const {
  return sensor_data.values[ALTITUDE];
}
