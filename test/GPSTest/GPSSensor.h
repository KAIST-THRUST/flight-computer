#ifndef GPSSensor_h
#define GPSSensor_h

#include <Adafruit_GPS.h>
#define GPSECHO false
#define GPSSerial Serial2


class GPSSensor {
public:
  GPSSensor();
  void begin();
  void update();

private:
  Adafruit_GPS GPS;
  uint32_t timer;
};

#endif
