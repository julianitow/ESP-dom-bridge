// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#ifndef SERIALLOGGER_H
#define SERIALLOGGER_H

#include <Arduino.h>
#include "../../include/common_config.h"
#include "../WebServer/WebServer.h"

#ifndef SERIAL_LOGGER_BAUD_RATE
#define SERIAL_LOGGER_BAUD_RATE 115200
#endif

class SerialLogger
{
private:
  static void print(String level, String message);
  static WebServer *server;

public:
  SerialLogger();
  static void initializeTime();
  void setWebServer(WebServer *server);
  void Debug(String message);
  void Error(String message);
  void Info(String message);
  void Warning(String message);
};

extern SerialLogger Logger;

#endif // SERIALLOGGER_H
