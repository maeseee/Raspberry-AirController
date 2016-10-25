#pragma once

#include <Gpio/IGpio.hpp>
#include <TimeTrigger.hpp>

#include <vector>

namespace night_air {

class NightAir {
public:
  NightAir(const gpio::IGpioPtr &gpio);

  gpio::Value getValue() const;

private:
  void addTimer(size_t onTime);

  gpio::IGpioPtr m_gpio;
  std::vector<time_trigger::TimeTriggerPtr> m_timers;
};
}
