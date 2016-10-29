#pragma once

#include <string>

namespace logger {

/**
 * @brief The SysLogger class logs to the file /var/log/syslog
 * TODO put the output to a seperate file
 */
class SysLogger {
public:
  static SysLogger instance();
  void log(const std::string &logMsg);

private:
  SysLogger();
};
}
