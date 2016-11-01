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

  /**
   * @brief time2Str converts a time in seconds to a readable time
   * @param time [s]
   * @return string of time
   */
  std::string time2Str(size_t time) const;

private:
  SysLogger();
};
}
