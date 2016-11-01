#include "SysLogger.hpp"
#include <Constants.hpp>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <syslog.h>

namespace logger {

SysLogger SysLogger::instance() {
  static SysLogger inst;
  return inst;
}

void SysLogger::log(const std::string &logMsg) {
  openlog("air_contr", LOG_PID | LOG_CONS, LOG_USER);
  syslog(LOG_INFO, "%s", logMsg.c_str());
  closelog();

  time_t t = time(0); // get time now
  struct tm *now = localtime(&t);
  size_t daytime =
      (now->tm_hour * HOUR_TO_SEC) + (now->tm_min * MIN_TO_SEC) + now->tm_sec;
  std::cout << "LogMsg (" << time2Str(daytime) << "): " << logMsg << std::endl;
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
}
