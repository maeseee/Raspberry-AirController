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

  size_t getTimeStamp() const;

private:
  size_t generateId();
  std::string getNameFromId(const size_t id) const;

  // log values
  std::map<size_t, std::string> m_idNames;
  std::map<size_t, gpio::Value> m_outputValues;

  struct logFloatEntry {
      size_t timeStamp;
      float value;
  };

  std::map<size_t, logFloatEntry> m_temperatureValues;
  std::map<size_t, logFloatEntry> m_humidityValues;

  // id generator
  std::atomic<size_t> m_idCounter;
};
using SysLoggerPtr = std::shared_ptr<SysLogger>;
}
