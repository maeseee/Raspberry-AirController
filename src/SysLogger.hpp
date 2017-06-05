#pragma once

#include <atomic>
#include <map>
#include <memory>
#include <string>
#include <vector>

// FWD
namespace gpio
{
enum class Value;
}

// Class
namespace logger
{
/**
 * @brief The SysLogger class logs to the file /var/log/syslog
 * TODO put the output to a seperate file
 */
class SysLogger
{
public:
    SysLogger();

    size_t generateId(const std::string& name);

    void logMsg(const size_t id, const std::string& logMessage);
    void logError(const size_t id, const std::string& logMessage);

    void logOutput(const size_t id, const gpio::Value value);
    void logSensorValue(const size_t id, const float value);

    /**
     * @brief time2Str converts a time in seconds to a readable time
     * @param time [s]
     * @return string of time
     */
    std::string time2Str(size_t time) const;

    std::vector<size_t> getIds(const std::string& name) const;
    std::vector<size_t> getAllIds() const;
    std::string getValueFromId(const size_t id) const;
    std::string getNameFromId(const size_t id) const;

private:
    void logMsg(const std::string& logMessage);
    std::string gpioValue2Str(const gpio::Value value);

    size_t newId();

    // log values
    std::map<size_t, std::string> m_idNames;
    std::map<size_t, std::string> m_values;

    // id generator
    std::atomic<size_t> m_nextId{1};
};
using SysLoggerPtr = std::shared_ptr<SysLogger>;
}
