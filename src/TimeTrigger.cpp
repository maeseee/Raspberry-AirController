#include "TimeTrigger.hpp"

#include <iostream>
#include <unistd.h>

namespace time_trigger {

static const size_t CALL_INTERVALL = 30; // call upcate intervall for thread


TimeTrigger::TimeTrigger(const size_t on, const size_t off, gpio::GpioSetter function)
    : m_period(OnPeriod{on, off})
    , fn_gpioSetter(function)
{
    m_thread = std::thread(&TimeTrigger::threadFn, this);
}

TimeTrigger::~TimeTrigger() {
  m_stopThread = true;
  m_thread.join();
}

void TimeTrigger::threadFn() {
  while (!m_stopThread) {
    time_t t = time(0); // get time now
    struct tm *now = localtime(&t);

    size_t daytime =
        (now->tm_hour * 60 * 60) + (now->tm_min * 60) + now->tm_sec;

    if (m_period.offTime < m_period.onTime) {
      if (daytime < m_period.onTime && daytime > m_period.offTime) {
          fn_gpioSetter(gpio::Value::OFF);
      } else {
          fn_gpioSetter(gpio::Value::ON);
      }
    } else {
      if (daytime < m_period.offTime && daytime > m_period.onTime) {
          fn_gpioSetter(gpio::Value::ON);
      } else {
          fn_gpioSetter(gpio::Value::OFF);
      }
    }

    sleep(CALL_INTERVALL);
  }
}
}
