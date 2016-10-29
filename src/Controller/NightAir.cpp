#include "NightAir.hpp"

#include <cassert>

namespace night_air {

static const size_t ON_TIME_DURATION =
    10 * 60; // call upcate intervall for thread
static const size_t ON_OFFSET_NIGHT_TARIF =
    15 * 60; // call upcate intervall for thread

NightAir::NightAir(const gpio::IGpioPtr &gpio) : m_gpio(gpio) {
  assert(m_gpio);

  addTimer(22 * 60 * 60 + ON_OFFSET_NIGHT_TARIF);
  addTimer(23 * 60 * 60 + ON_OFFSET_NIGHT_TARIF);
  addTimer(0 * 60 * 60 + ON_OFFSET_NIGHT_TARIF);
  addTimer(1 * 60 * 60 + ON_OFFSET_NIGHT_TARIF);
  addTimer(2 * 60 * 60 + ON_OFFSET_NIGHT_TARIF);
  addTimer(3 * 60 * 60 + ON_OFFSET_NIGHT_TARIF);
  addTimer(4 * 60 * 60 + ON_OFFSET_NIGHT_TARIF);
  addTimer(5 * 60 * 60 + ON_OFFSET_NIGHT_TARIF);
  addTimer(6 * 60 * 60 + ON_OFFSET_NIGHT_TARIF);
}

void NightAir::addTimer(size_t onTime) {
  time_trigger::TimeTriggerPtr timer =
      std::make_shared<time_trigger::TimeTrigger>(
          onTime, onTime + ON_TIME_DURATION, m_gpio);
  m_timers.push_back(timer);
}
}
