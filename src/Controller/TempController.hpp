#pragma once

#include <Gpio/IGpio.hpp>
#include <Threading.hpp>
#include <TimeTrigger.hpp>

namespace controller {

class TempController : public threading::Threading {
public:
  /**
 * @brief TemperatureController turns on and off the air system
 * @param gpioMainSystem gpio for switching on and off
 */
  TempController(const gpio::IGpioPtr &gpioMainSystem);

  void recall() override;

private:
  bool shouldWarm() const;

  gpio::IGpioPtr m_gpio;
  time_trigger::TimeTriggerPtr m_timer;
  bool m_oldShouldWarmup{true};
};
}
