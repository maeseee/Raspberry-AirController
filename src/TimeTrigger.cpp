#include "TimeTrigger.hpp"
#include <Constants.hpp>

#include <ctime>
#include <unistd.h> // for sleep

namespace time_trigger {

TimeTrigger::TimeTrigger(const size_t on, const size_t off,
                         const gpio::IGpioPtr &gpio)
    : m_period(OnPeriod{on, off}), m_gpio(gpio) {
  m_thread = std::thread(&TimeTrigger::threadFn, this);
}

TimeTrigger::~TimeTrigger() {
  m_stopThread = true;
  m_thread.join();
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

void TimeTrigger::threadFn() {
  while (!m_stopThread) {
    static int timeCounter = 0;
    if (0 == timeCounter) {
      recall();
    }
    ++timeCounter;
    timeCounter %= CALL_INTERVALL_TIMER;
    sleep(1);
  }
}

void TimeTrigger::recall() { m_gpio->setValue(getValue()); }
}
