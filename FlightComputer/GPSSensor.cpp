#include "GPSSensor.h"

GPSSensor::GPSSensor() : GPS(&GPS_SERIAL), is_fixed(false) {
  sensor_data.data_count = DATA_COUNT;
}

void GPSSensor::begin(float *ptr) {
  data_ptr = ptr;
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

  if (current_time - last_update_time < (1000 / GPS_SAMPLING_RATE)) {
    data_ptr[IS_UPDATED] = 0;
    return;
  }
  last_update_time = current_time;

  if (GPS.fix) {
    /* If GPS is fixed, set is_fixed to true. */
    if (!is_fixed) {
      is_fixed = true;
    }
    /* Direction of latitude and longitude in sign. */
    int lat_dir = (GPS.lat == 'N') ? 1 : -1;
    int lon_dir = (GPS.lon == 'E') ? 1 : -1;

    data_ptr[LATITUDE] = GPS.latitudeDegrees * lat_dir;
    data_ptr[LONGITUDE] = GPS.longitudeDegrees * lon_dir;
    data_ptr[ALTITUDE] = GPS.altitude;
    data_ptr[GEOID_HEIGHT] = GPS.geoidheight;
    data_ptr[IS_UPDATED] = 1;
  }
}

String GPSSensor::toString() const {
  return "[GPS] Latitude: " + String(data_ptr[LATITUDE], 7) +
         ", Longitude: " + String(data_ptr[LONGITUDE], 7) +
         ", Altitude: " + String(data_ptr[ALTITUDE], 7) +
         ", Average Altitude: " + String(data_ptr[ALTITUDE_LS], 7);
}

bool GPSSensor::isValid() const {
  return is_fixed && abs(data_ptr[ALTITUDE]) > 0.0001 &&
         abs(data_ptr[GEOID_HEIGHT]) > 0.0001;
}

float GPSSensor::getLatitude() const { return data_ptr[LATITUDE]; }

float GPSSensor::getLongitude() const { return data_ptr[LONGITUDE]; }

float GPSSensor::getAltitude() const { return data_ptr[ALTITUDE]; }
