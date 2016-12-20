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

  size_t generateId(const std::string &name);

  void logMsg(const size_t id, const std::string &logMessage);
  void logError(const size_t id, const std::string &logMessage);

  void logOutput(const size_t id, const gpio::Value value);
  void logSensorTemperature(const size_t id, const float temperature);

  void logSensorHumidity(const size_t id, const float humidity);

  /**
   * @brief time2Str converts a time in seconds to a readable time
   * @param time [s]
   * @return string of time
   */
  std::string time2Str(size_t time) const;

private:
  void logMsg(const std::string &logMessage);

  size_t newId();
  std::string getNameFromId(const size_t id) const;

  // log values
  std::map<size_t, std::string> m_idNames;

  std::map<size_t, gpio::Value> m_outputValues;
  std::map<size_t, float> m_temperatureValues;
  std::map<size_t, float> m_humidityValues;

  // id generator
  std::atomic<size_t> m_nextId{1};
};
using SysLoggerPtr = std::shared_ptr<SysLogger>;
}
