// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#ifndef SERIALLOGGER_H
#define SERIALLOGGER_H

#include <Arduino.h>
#include <WebSerial.h>
#include <WebServer.h>
#include "../../include/common_config.h"

#ifndef SERIAL_LOGGER_BAUD_RATE
#define SERIAL_LOGGER_BAUD_RATE 115200
#endif

class SerialLogger
{
private:
  static void print(String level, String message);

public:
  SerialLogger();
  static void initializeTime();
  void Debug(String message);
  void Error(String message);
  void Info(String message);
  void Warning(String message);
};

extern SerialLogger Logger;

#endif // SERIALLOGGER_H
