#include "TimeTrigger.hpp"
#include <Constants.hpp>
#include <Controller/ControllerIdGenerator.hpp>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace time_trigger {

TimeTrigger::TimeTrigger(const size_t on, const size_t off,
                         const gpio::IGpioPtr &gpio)
    : threading::Threading(CALL_INTERVALL_TIMER), m_onTime(on), m_offTime(off),
      m_gpio(gpio) {
  std::cout << "Add TimeTrigger with on at " << time2Str(on) << " and off at  "
            << time2Str(off) << std::endl;

  m_controllerId = controller::IdGenerator::generateId();
}

TimeTrigger::~TimeTrigger() {
  m_gpio->setValue(m_controllerId, gpio::Value::LOW);
}

gpio::Value TimeTrigger::getValue() const {
  time_t t = time(0); // get time now
  struct tm *now = localtime(&t);
  size_t daytime =
      (now->tm_hour * HOUR_TO_SEC) + (now->tm_min * MIN_TO_SEC) + now->tm_sec;
  std::cout << time2Str(daytime) << std::endl;

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

void TimeTrigger::recall() {
  // Invalid is not really true but it does not matter
  m_gpio->setValue(m_controllerId, gpio::Value::INVALID);
}

std::string TimeTrigger::time2Str(size_t time) const {

  size_t timeMin = time / 60;
  size_t timeH = timeMin / 60;
  timeMin = timeMin - (timeH * MIN_TO_SEC);

  std::stringstream ss;
  ss << timeH << ":" << std::setfill('0') << std::setw(2) << timeMin;
  return ss.str();
}
}
