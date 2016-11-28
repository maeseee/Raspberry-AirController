#include "NightAir.hpp"
#include <Constants.hpp>
#include <Controller/TimeTrigger.hpp>

#include <cassert>

namespace controller {

static const size_t ON_TIME_DURATION =
    10 * MIN_TO_SEC; // call upcate intervall for thread
static const size_t ON_OFFSET_NIGHT_TARIF =
    15 * MIN_TO_SEC; // call upcate intervall for thread

NightAir::NightAir(const gpio::IGpioPtr &gpio,
                   const logger::SysLoggerPtr &sysLogger)
    : m_gpio(gpio), m_sysLogger(sysLogger) {
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
          onTime, onTime + ON_TIME_DURATION, m_gpio, "Night hourly air",
          m_sysLogger);
  m_timers.push_back(timer);
}
}
