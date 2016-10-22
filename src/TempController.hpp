#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <IGpio.hpp>
#include <Threading.hpp>
#include <TimeTrigger.hpp>

namespace temp_controller {

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

} // humidity
#endif
