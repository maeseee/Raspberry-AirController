#ifndef SYSLOGGER_HPP
#define SYSLOGGER_HPP

#include <iostream>

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

} // namespace safety

#endif // SAFETYLOGICLIB_SRC_UTILITY_SYSLOGGER_H_
