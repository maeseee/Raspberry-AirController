#include "TempController.hpp"
#include <Controller/TimeTrigger.hpp>
#include <Gpio/IGpio.hpp>
#include <SysLogger.hpp>
#include <Utility/Constants.hpp>

#include <cassert>

namespace controller
{

static const size_t SUMMER_ON = 1 * HOUR_TO_SEC;
static const size_t SUMMER_ON_DURATION = 6 * HOUR_TO_SEC;
static const size_t WINTER_ON = 13 * HOUR_TO_SEC;
static const size_t WINTER_ON_DURATION = 2 * HOUR_TO_SEC;

TempController::TempController(const gpio::IGpioPtr& gpioMainSystem, const logger::SysLoggerPtr& sysLogger)
    : threading::Threading(CALL_INTERVALL_TEMP)
    , m_gpio(gpioMainSystem)
    , m_sysLogger(sysLogger)
    , m_loggerId(sysLogger->generateId("TempController"))
{
    assert(m_gpio);

    setInitialized();
}

void TempController::recall()
{
    const bool isShouldWarm = shouldWarm();
    if ((nullptr == m_timer) || (m_oldShouldWarmup != isShouldWarm)) {
        m_timer = nullptr; // Destruct current timer
        if (isShouldWarm) {
            m_timer = std::make_shared<time_trigger::TimeTrigger>(WINTER_ON, WINTER_ON + WINTER_ON_DURATION, m_gpio,
                                                                  "Winter fresh air", m_sysLogger);
        } else {
            m_timer = std::make_shared<time_trigger::TimeTrigger>(SUMMER_ON, SUMMER_ON + SUMMER_ON_DURATION, m_gpio,
                                                                  "Summer fresh air", m_sysLogger);
        }
        m_oldShouldWarmup = isShouldWarm;
    }
}
}
