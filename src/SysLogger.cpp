#include "SysLogger.hpp"

#include <syslog.h>

namespace logger {

SysLogger SysLogger::instance() {
  static SysLogger inst;
  return inst;
}

void SysLogger::log(const std::string &logMsg) {
  openlog("air_contr", LOG_PID | LOG_CONS, LOG_USER);
  std::cout << "LogMsg: " << logMsg << std::endl;
  syslog(LOG_INFO, "%s", logMsg.c_str());
  closelog();
}

SysLogger::SysLogger() {}
}
