#pragma once

#include <atomic>
#include <map>
#include <memory>
#include <string>

// FWD
namespace gpio {
enum class Value : size_t;
}

// Class
namespace logger {
/**
 * @brief The SysLogger class logs to the file /var/log/syslog
 * TODO put the output to a seperate file
 */
class SysLogger {
public:
  SysLogger();

  size_t getId(const std::string &name);

  void logMsg(const std::string &logMsg);

  void logOutput(const size_t id, const gpio::Value value);
  // void logInput(const size_t id, const gpio::Value value);

  /**
   * @brief time2Str converts a time in seconds to a readable time
   * @param time [s]
   * @return string of time
   */
  std::string time2Str(size_t time) const;

private:
  size_t generateId();

  // log values
  std::map<size_t, std::string> m_idNames;
  std::map<size_t, gpio::Value> m_outputValues;

  // id generator
  std::atomic<size_t> m_idCounter;
};
using SysLoggerPtr = std::shared_ptr<SysLogger>;
}
