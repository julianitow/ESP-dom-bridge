// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#include "SerialLogger.h"
#include <time.h>

#define UNIX_EPOCH_START_YEAR 1900

static void writeTime()
{
  struct tm *ptm;
  time_t now = time(NULL);

  ptm = gmtime(&now);

  Serial.print(ptm->tm_year + UNIX_EPOCH_START_YEAR);
  Serial.print("/");
  Serial.print(ptm->tm_mon + 1);
  Serial.print("/");
  Serial.print(ptm->tm_mday);
  Serial.print(" ");

  if (ptm->tm_hour < 10)
  {
    Serial.print(0);
  }

  Serial.print(ptm->tm_hour);
  Serial.print(":");

  if (ptm->tm_min < 10)
  {
    Serial.print(0);
  }

  Serial.print(ptm->tm_min);
  Serial.print(":");

  if (ptm->tm_sec < 10)
  {
    Serial.print(0);
  }

  Serial.print(ptm->tm_sec);
}

void SerialLogger::initializeTime()
{
  Logger.Info("Setting time using SNTP");
  configTime(GMT_OFFSET_SECS, GMT_OFFSET_SECS_DST, NTP_SERVERS);
  time_t now = time(NULL);
  while (now < UNIX_TIME_NOV_13_2017)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  Logger.Info("Time initialized!");
}

/*void SerialLogger::setWebServer(WebServer *server)
{
  SerialLogger::server = server;
}*/

SerialLogger::SerialLogger() { Serial.begin(SERIAL_LOGGER_BAUD_RATE); }

void SerialLogger::print(String level, String message)
{
  writeTime();
  Serial.print(level);
  Serial.println(message);
}

void SerialLogger::Debug(String message)
{
  SerialLogger::print(" [DEBUG] ", message);
}

void SerialLogger::Error(String message)
{
  SerialLogger::print(" [ERROR] ", message);
}

void SerialLogger::Info(String message)
{
  SerialLogger::print(" [INFO] ", message);
}

void SerialLogger::Warning(String message)
{
  SerialLogger::print(" [WARNING] ", message);
}

SerialLogger Logger;
