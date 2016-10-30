#pragma once

#include <Gpio/IGpio.hpp>
#include <Threading.hpp>

#include <string>

namespace time_trigger {

class TimeTrigger : public threading::Threading {
public:
  /**
 * @brief TimeTrigger calls the std::function in the specific intervall
 * @param on [s] from 00:00
 * @param off [s] from 00:00
 */
  TimeTrigger(const size_t on, const size_t off, const gpio::IGpioPtr &gpio);
  ~TimeTrigger();

  gpio::Value getValue() const;

  void recall() override;

private:
  /**
   * @brief time2Str converts a time in seconds to a readable time
   * @param time [s]
   * @return string of time
   */
  std::string time2Str(size_t time) const;

  size_t m_onTime;  // [s]
  size_t m_offTime; // [s]
  gpio::IGpioPtr m_gpio;
  size_t m_controllerId;
};

using TimeTriggerPtr = std::shared_ptr<TimeTrigger>;
}
