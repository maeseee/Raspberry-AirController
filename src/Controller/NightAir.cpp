#include "NightAir.hpp"
#include <Constants.hpp>

#include <cassert>

namespace night_air {

static const size_t ON_TIME_DURATION =
    10 * MIN_TO_SEC; // call upcate intervall for thread
static const size_t ON_OFFSET_NIGHT_TARIF =
    15 * MIN_TO_SEC; // call upcate intervall for thread

NightAir::NightAir(const gpio::IGpioPtr &gpio) : m_gpio(gpio) {
  assert(m_gpio);

  addTimer(22 * HOUR_TO_SEC + ON_OFFSET_NIGHT_TARIF);
  addTimer(23 * HOUR_TO_SEC + ON_OFFSET_NIGHT_TARIF);
  addTimer(0 * HOUR_TO_SEC + ON_OFFSET_NIGHT_TARIF);
  addTimer(1 * HOUR_TO_SEC + ON_OFFSET_NIGHT_TARIF);
  addTimer(2 * HOUR_TO_SEC + ON_OFFSET_NIGHT_TARIF);
  addTimer(3 * HOUR_TO_SEC + ON_OFFSET_NIGHT_TARIF);
  addTimer(4 * HOUR_TO_SEC + ON_OFFSET_NIGHT_TARIF);
  addTimer(5 * HOUR_TO_SEC + ON_OFFSET_NIGHT_TARIF);
  addTimer(6 * HOUR_TO_SEC + ON_OFFSET_NIGHT_TARIF);
}

void NightAir::addTimer(size_t onTime) {
  time_trigger::TimeTriggerPtr timer =
      std::make_shared<time_trigger::TimeTrigger>(
          onTime, onTime + ON_TIME_DURATION, m_gpio);
  m_timers.push_back(timer);
}
}
