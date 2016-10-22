#include "TimeTrigger.hpp"
#include <Constants.hpp>

#include <ctime>

namespace time_trigger {

TimeTrigger::TimeTrigger(const size_t on, const size_t off,
                         const gpio::IGpioPtr &gpio)
    : threading::Threading(CALL_INTERVALL_TIMER), m_period(OnPeriod{on, off}),
      m_gpio(gpio) {
}

gpio::Value TimeTrigger::getValue() const {
  time_t t = time(0); // get time now
  struct tm *now = localtime(&t);
  size_t daytime = (now->tm_hour * 60 * 60) + (now->tm_min * 60) + now->tm_sec;

  gpio::Value result;
  if (m_period.offTime < m_period.onTime) {
    if (daytime < m_period.onTime && daytime > m_period.offTime) {
      result = gpio::Value::LOW;
    } else {
      result = gpio::Value::HIGH;
    }
  } else {
    if (daytime < m_period.offTime && daytime > m_period.onTime) {
      result = gpio::Value::HIGH;
    } else {
      result = gpio::Value::LOW;
    }
  }
  return result;
}

void TimeTrigger::recall() { m_gpio->setValue(getValue()); }
}
