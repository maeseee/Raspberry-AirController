#ifndef TIME_TRIGGER_H
#define TIME_TRIGGER_H

#include <IGpio.hpp>

#include <thread>

namespace time_trigger {

struct OnPeriod {
  size_t onTime;  // [s]
  size_t offTime; // [s]
};

class TimeTrigger {
public:
  /**
 * @brief TimeTrigger calls the std::function in the specific intervall
 * @param on [s] from 00:00
 * @param off [s] from 00:00
 */
  TimeTrigger(const size_t on, const size_t off, const gpio::IGpioPtr &gpio);
  ~TimeTrigger();

private:
  void threadFn();
  void recall();

  std::thread m_thread;
  bool m_stopThread{false};

  OnPeriod m_period;
  gpio::IGpioPtr m_gpio;
};

} // time_trigger

#endif // TIME_TRIGGER_H
