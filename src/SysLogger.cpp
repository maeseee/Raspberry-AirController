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

size_t SysLogger::getId(const std::string &name) {
  size_t id = generateId();
  m_idNames[id] = name;
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

void SysLogger::logOutput(const size_t id, const gpio::Value value) {
  assert(0 != id && "Invalid id. Try to register this task");

  m_outputValues[id] = value;

  std::stringstream ss;
  ss << "GPIO ";
  if (m_idNames.count(id)) {
    ss << m_idNames[id] << " (" << id << ")";
  } else {
    ss << id;
  }

  ss << " has been turn " << ((gpio::Value::HIGH == value) ? "on" : "off");
  logMsg(ss.str());

  m_outputValues[id] = value;
}

std::string SysLogger::time2Str(size_t time) const {

  size_t timeMin = time / 60;
  size_t timeH = timeMin / 60;
  timeMin = timeMin - (timeH * MIN_TO_SEC);

  std::stringstream ss;
  ss << timeH << ":" << std::setfill('0') << std::setw(2) << timeMin;
  return ss.str();
}

SysLogger::SysLogger() {}

size_t SysLogger::generateId() {
  size_t expectedId;
  size_t desiredId;
  do {
    expectedId = m_idCounter.load();
    desiredId = expectedId + 1;
  } while (!m_idCounter.compare_exchange_strong(expectedId, desiredId));

  return desiredId;
}
}
