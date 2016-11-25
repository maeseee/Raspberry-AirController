#include "TimeTrigger.hpp"
#include <Constants.hpp>
#include <Controller/ControllerIdGenerator.hpp>
#include <SysLogger.hpp>

#include <ctime>

namespace time_trigger {

TimeTrigger::TimeTrigger(const size_t on, const size_t off,
                         const gpio::IGpioPtr &gpio)
    : threading::Threading(CALL_INTERVALL_TIMER), m_onTime(on), m_offTime(off),
      m_gpio(gpio) {
  logger::SysLogger::instance().log(
      "Add TimeTrigger for " + std::to_string(gpio->getPinNumber()) +
      " with on at " + logger::SysLogger::instance().time2Str(on) +
      " and off at  " + logger::SysLogger::instance().time2Str(off));

  m_controllerId = controller::IdGenerator().generateId("TimeTrigger");
}

TimeTrigger::~TimeTrigger() {
  m_gpio->setValue(m_controllerId, gpio::Value::LOW);
}

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

void TimeTrigger::recall() { m_gpio->setValue(m_controllerId, getValue()); }
}
