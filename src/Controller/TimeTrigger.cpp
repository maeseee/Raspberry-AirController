#include "TimeTrigger.hpp"
#include <Constants.hpp>
#include <SysLogger.hpp>

#include <ctime>

namespace time_trigger
{

TimeTrigger::TimeTrigger(const size_t on,
                         const size_t off,
                         const gpio::IGpioPtr& gpio,
                         const std::string& name,
                         const logger::SysLoggerPtr& sysLogger)
    : threading::Threading(CALL_INTERVALL_TIMER)
    , m_onTime(on)
    , m_offTime(off)
    , m_gpio(gpio)
    , m_sysLogger(sysLogger)
    , m_loggerId(sysLogger->generateId("TimeTrigger " + name))
{
    m_sysLogger->logMsg(m_loggerId, "Add TimeTrigger for gpio " + std::to_string(gpio->getPinNumber()) + ": on at " +
                                        m_sysLogger->time2Str(on) + " and off at  " + m_sysLogger->time2Str(off));
}

TimeTrigger::~TimeTrigger()
{
    m_gpio->setValue(m_loggerId, gpio::Value::LOW);
}

bool TimeTrigger::inTimeRange() const
{
    time_t t = time(0); // get time now
    struct tm* now = localtime(&t);
    size_t daytime = (now->tm_hour * HOUR_TO_SEC) + (now->tm_min * MIN_TO_SEC) + now->tm_sec;

    bool isInTimeRange = false;
    if (m_offTime < m_onTime) {
        if (daytime < m_onTime && daytime > m_offTime) {
            isInTimeRange = false;
        } else {
            isInTimeRange = true;
        }
    } else {
        if (daytime < m_offTime && daytime > m_onTime) {
            isInTimeRange = true;
        } else {
            isInTimeRange = false;
        }
    }
    return isInTimeRange;
}

void TimeTrigger::recall()
{
    gpio::Value value = gpio::Value::LOW;
    if (inTimeRange()) {
        value = gpio::Value::HIGH;
    }
    m_gpio->setValue(m_loggerId, value);
}
}
