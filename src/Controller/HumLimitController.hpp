#pragma once

#include <Gpio/IGpio.hpp>
#include <Sensor/ISensor.hpp>
#include <Threading.hpp>

// FWD
namespace logger {
class SysLogger;
using SysLoggerPtr = std::shared_ptr<SysLogger>;
}

namespace time_trigger {
class TimeTrigger;
using TimeTriggerPtr = std::shared_ptr<TimeTrigger>;
}

// Class
namespace controller {

class HumLimitController : public threading::Threading {
public:
  /**
 * @brief TemperatureController turns on and off the air system
 * @param gpioMainSystem gpio for switching on and off
 */
  HumLimitController(const sensor::ISensorPtr &indoorSensor,
                     const sensor::ISensorPtr &outdoorSensor,
                     const gpio::IGpioPtr &gpioMainSystem,
                     const logger::SysLoggerPtr &sysLogger);

  void recall() override;

private:
  bool shouldWarm() const;

  sensor::ISensorPtr m_indoorSensor;
  sensor::ISensorPtr m_outdoorSensor;
  gpio::IGpioPtr m_gpio;

  time_trigger::TimeTriggerPtr m_timer;

  const logger::SysLoggerPtr m_sysLogger;
  size_t m_loggerId{0};
};
}