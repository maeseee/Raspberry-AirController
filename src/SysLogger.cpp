#include "SysLogger.hpp"
#include <Constants.hpp>
#include <Gpio/IGpio.hpp>

#include <cassert>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <syslog.h>

namespace logger {

SysLogger::SysLogger() {}

size_t SysLogger::generateId(const std::string &name) {
  size_t id = newId();
  m_idNames[id] = name;
  return id;
}

void SysLogger::logMsg(const size_t id, const std::string &logMessage) {
  std::string logInfo = getNameFromId(id) + " mentioned that: " + logMessage;
  logMsg(logInfo);
}

void SysLogger::logError(const size_t id, const std::string &logMessage) {
  std::string logInfo =
      "Error: " + getNameFromId(id) + " mentioned that: " + logMessage;
  logMsg(logInfo);
}

void SysLogger::logOutput(const size_t id, const gpio::Value value) {
  // save data for later use
  m_outputValues[id] = value;

  // log it
  std::string logInfo = "GPIO " + getNameFromId(id) + " has been turned " +
                        ((gpio::Value::HIGH == value) ? "on" : "off");
  logMsg(logInfo);
}

void SysLogger::logSensorTemperature(const size_t id, const float temperature) {
  // save data for later use
  m_temperatureValues[id] = temperature;

  // log it
  std::string logInfo = "Temperature sensor " + getNameFromId(id) +
                        " caught the value " + std::to_string(temperature) +
                        "C";
  logMsg(logInfo);
}

void SysLogger::logSensorHumidity(const size_t id, const float humidity) {
  // save data for later use
  m_humidityValues[id] = humidity;

  // log it
  std::string logInfo = "Humidity sensor " + getNameFromId(id) +
                        " caught the value " + std::to_string(humidity) + "%";
  logMsg(logInfo);
}

std::string SysLogger::time2Str(size_t time) const {

  size_t timeMin = time / 60;
  size_t timeH = timeMin / 60;
  timeMin = timeMin - (timeH * MIN_TO_SEC);

  std::stringstream ss;
  ss << timeH << ":" << std::setfill('0') << std::setw(2) << timeMin;
  return ss.str();
}

void SysLogger::logMsg(const std::string &logMsg) {
  openlog("air_contr", LOG_PID | LOG_CONS, LOG_USER);
  syslog(LOG_INFO, "%s", logMsg.c_str());
  closelog();

  time_t t = time(0); // get time now
  struct tm *now = localtime(&t);
  size_t daytime =
      (now->tm_hour * HOUR_TO_SEC) + (now->tm_min * MIN_TO_SEC) + now->tm_sec;
  std::cout << "LogMsg (" << time2Str(daytime) << "): " << logMsg << std::endl;
}

size_t SysLogger::newId() {
  size_t expectedId;
  size_t desiredId;
  do {
    expectedId = m_nextId.load();
    desiredId = expectedId + 1;
  } while (!m_nextId.compare_exchange_strong(expectedId, desiredId));

  return expectedId;
}

std::string SysLogger::getNameFromId(const size_t id) const {
  assert(0 != id && "Invalid id. Try to register this task");

  if (m_idNames.count(id)) {
    return m_idNames.at(id) + " (" + std::to_string(id) + ")";
  } else {
    return std::to_string(id);
  }
}
}
