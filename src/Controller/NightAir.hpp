#pragma once

#include <Controller/TimeTrigger.hpp>
#include <Gpio/IGpio.hpp>

#include <vector>

namespace controller {

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
