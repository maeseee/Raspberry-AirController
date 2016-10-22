#ifndef TIME_TRIGGER_H
#define TIME_TRIGGER_H

#include <IGpio.hpp>
#include <Threading.hpp>

namespace time_trigger {

struct OnPeriod {
  size_t onTime;  // [s]
  size_t offTime; // [s]
};

class TimeTrigger : public threading::Threading{
public:
  /**
 * @brief TimeTrigger calls the std::function in the specific intervall
 * @param on [s] from 00:00
 * @param off [s] from 00:00
 */
  TimeTrigger(const size_t on, const size_t off, const gpio::IGpioPtr &gpio);

  gpio::Value getValue() const;

  void recall() override;
private:

  OnPeriod m_period;
  gpio::IGpioPtr m_gpio;
};

using TimeTriggerPtr = std::shared_ptr<TimeTrigger>;

} // time_trigger

#endif // TIME_TRIGGER_H
