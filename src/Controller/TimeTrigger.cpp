#include "TimeTrigger.hpp"
#include <Constants.hpp>
#include <SysLogger.hpp>

#include <ctime>

namespace time_trigger {

TimeTrigger::TimeTrigger(const size_t on, const size_t off,
                         const gpio::IGpioPtr &gpio,
                         const logger::SysLoggerPtr &sysLogger)
    : threading::Threading(CALL_INTERVALL_TIMER), m_onTime(on), m_offTime(off),
      m_gpio(gpio), m_sysLogger(sysLogger) {
  m_loggerId = m_sysLogger->getId("TimeTrigger");

  m_sysLogger->logMsg(m_loggerId, "Add TimeTrigger for gpio " +
                                      std::to_string(gpio->getPinNumber()) +
                                      ": on at " + m_sysLogger->time2Str(on) +
                                      " and off at  " +
                                      m_sysLogger->time2Str(off));
}

TimeTrigger::~TimeTrigger() { m_gpio->setValue(m_loggerId, gpio::Value::LOW); }

gpio::Value TimeTrigger::getValue() const {
  time_t t = time(0); // get time now
  struct tm *now = localtime(&t);
  size_t daytime =
      (now->tm_hour * HOUR_TO_SEC) + (now->tm_min * MIN_TO_SEC) + now->tm_sec;

  gpio::Value result;
  if (m_offTime < m_onTime) {
    if (daytime < m_onTime && daytime > m_offTime) {
      result = gpio::Value::LOW;
    } else {
      result = gpio::Value::HIGH;
    }
  } else {
    if (daytime < m_offTime && daytime > m_onTime) {
      result = gpio::Value::HIGH;
    } else {
      result = gpio::Value::LOW;
    }
  }
  return result;
}

void TimeTrigger::recall() { m_gpio->setValue(m_loggerId, getValue()); }
}
