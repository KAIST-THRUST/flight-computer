#ifndef LOGGER_H
#define LOGGER_H

#include "DefaultLogFormatter.h"
#include "Navigation.h"
#include "SensorSet.h"
#include "SerialDevice.h"

class Logger {
public:
  Logger();
  void begin();
};

#endif
