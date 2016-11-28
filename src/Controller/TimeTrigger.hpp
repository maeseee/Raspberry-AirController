#pragma once

#include <Gpio/IGpio.hpp>
#include <Threading.hpp>

#include <string>

// FWD
namespace logger {
class SysLogger;
using SysLoggerPtr = std::shared_ptr<SysLogger>;
}

// Class
namespace time_trigger {

class TimeTrigger : public threading::Threading {
public:
  /**
 * @brief TimeTrigger calls the std::function in the specific intervall
 * @param on [s] from 00:00
 * @param off [s] from 00:00
 */
  TimeTrigger(const size_t on, const size_t off, const gpio::IGpioPtr &gpio,
              const std::string &name, const logger::SysLoggerPtr &sysLogger);
  ~TimeTrigger();

  gpio::Value getValue() const;

  void recall() override;

private:
  size_t m_onTime;  // [s]
  size_t m_offTime; // [s]
  gpio::IGpioPtr m_gpio;

  const logger::SysLoggerPtr m_sysLogger;
  size_t m_loggerId{0};
};

using TimeTriggerPtr = std::shared_ptr<TimeTrigger>;
}
