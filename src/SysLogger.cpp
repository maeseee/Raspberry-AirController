#include "SysLogger.hpp"
#include <Gpio/IGpio.hpp>
#include <Utility/Constants.hpp>

#include <cassert>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <syslog.h>

namespace logger
{

size_t SysLogger::generateId(const std::string& name)
{
    size_t id = newId();
    m_idNames[id] = name;

    logMsg(id, "Newly generated");
    return id;
}

void SysLogger::logMsg(const size_t id, const std::string& logMessage)
{
    std::string logInfo = getNameFromId(id) + " mentioned that: " + logMessage;
    logMsg(logInfo);
}

void SysLogger::logError(const size_t id, const std::string& logMessage)
{
    std::string logInfo = "Error: " + getNameFromId(id) + " mentioned that: " + logMessage;
    logMsg(logInfo);
}

void SysLogger::logOutput(const size_t id, const gpio::Value value)
{
    // save data for later use
    m_values[id] = gpioValue2Str(value);

    // log it
    std::string logInfo =
        "GPIO " + getNameFromId(id) + " has been turned " + ((gpio::Value::HIGH == value) ? "on" : "off");
    logMsg(logInfo);
}

void SysLogger::logSensorValue(const size_t id, const double value)
{
    std::string lastValue = "-";

    if (0 < m_values.count(id)) {
        // value is already in the map
        lastValue = m_values[id];
    }

    // save data anyway for later use
    m_values[id] = std::to_string(value);

    // log it only on changes
    if (m_values[id] != lastValue) {
        std::string logInfo = "Sensor value of " + getNameFromId(id) + " caught the value " + std::to_string(value);
        logMsg(logInfo);
    }
}

std::string SysLogger::time2Str(size_t time) const
{

    size_t timeMin = time / 60;
    size_t timeH = timeMin / 60;
    timeMin = timeMin - (timeH * MIN_TO_SEC);

    std::stringstream ss;
    ss << timeH << ":" << std::setfill('0') << std::setw(2) << timeMin;
    return ss.str();
}

std::vector<size_t> SysLogger::getIds(const std::string& name) const
{
    std::vector<size_t> ids;
    for (auto& entry : m_idNames) {
        if (std::string::npos != entry.second.find(name)) {
            ids.push_back(entry.first);
        }
    }
    return ids;
}

std::vector<size_t> SysLogger::getAllIds() const
{
    std::vector<size_t> ids;
    for (auto& entry : m_idNames) {
        ids.push_back(entry.first);
    }
    return ids;
}

std::string SysLogger::getValueFromId(const size_t id) const
{
    if (0 < m_values.count(id)) {
        return m_values.at(id);
    } else {
        return "";
    }
}

void SysLogger::logMsg(const std::string& logMsg)
{
    openlog("air_contr", LOG_PID | LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "%s", logMsg.c_str());
    closelog();

    time_t t = time(nullptr); // get time now
    struct tm* now = localtime(&t);
    std::stringstream date;
    date << (now->tm_mday) << "." << (now->tm_mon + 1) << "." << (now->tm_year + 1900) << ":"
         << time2Str((now->tm_hour * HOUR_TO_SEC) + (now->tm_min * MIN_TO_SEC) + now->tm_sec);
    std::cout << "LogMsg (" << date.str() << "): " << logMsg << std::endl;
}

std::string SysLogger::gpioValue2Str(const gpio::Value value)
{
    switch (value) {
    case gpio::Value::LOW:
        return "GPIO is low";
    case gpio::Value::HIGH:
        return "GPIO is high";
    case gpio::Value::INVALID:
        return "GPIO not set";
    default:
        return "ERROR should never been called";
    }
}

size_t SysLogger::newId()
{
    size_t expectedId;
    size_t desiredId;
    do {
        expectedId = m_nextId.load();
        desiredId = expectedId + 1;
    } while (!m_nextId.compare_exchange_strong(expectedId, desiredId));

    return expectedId;
}

std::string SysLogger::getNameFromId(const size_t id) const
{
    assert(0 != id && "Invalid id. Try to register this task");

    if (0 < m_idNames.count(id)) {
        return m_idNames.at(id) + " (" + std::to_string(id) + ")";
    } else {
        return std::to_string(id);
    }
}
}
