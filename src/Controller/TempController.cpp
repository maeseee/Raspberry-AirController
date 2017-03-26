#include "TempController.hpp"
#include <Constants.hpp>
#include <Controller/TimeTrigger.hpp>
#include <SysLogger.hpp>

#include <cassert>

namespace controller
{

static const size_t SUMMER_ON = 2 * HOUR_TO_SEC;
static const size_t WINTER_ON = 13 * HOUR_TO_SEC;
static const size_t ON_DURATION = 2 * HOUR_TO_SEC;

TempController::TempController(const gpio::IGpioPtr& gpioMainSystem, const logger::SysLoggerPtr& sysLogger)
    : threading::Threading(CALL_INTERVALL_TEMP)
    , m_gpio(gpioMainSystem)
    , m_sysLogger(sysLogger)
    , m_loggerId(sysLogger->generateId("TempController"))
{
    assert(m_gpio);
}

bool TempController::shouldWarm() const
{
    time_t t = time(0); // get time now
    struct tm* now = localtime(&t);

    // Example of date of today
    //  std::cout << "Date of today is: " << (now->tm_year + 1900) << '-'
    //            << (now->tm_mon + 1) << '-' << now->tm_mday << std::endl;

    size_t month = now->tm_mon + 1;
    if (month < 7 && month > 8) {
        // Cool in the months July and August
        return false;
    } else {
        return true;
    }
}

void TempController::recall()
{
    const bool isShouldWarm = shouldWarm();
    if ((nullptr == m_timer) || (m_oldShouldWarmup != isShouldWarm)) {
        m_timer = nullptr; // Destruct current timer
        if (isShouldWarm) {
            m_timer = std::make_shared<time_trigger::TimeTrigger>(WINTER_ON, WINTER_ON + ON_DURATION, m_gpio,
                                                                  "Winter fresh air", m_sysLogger);
        } else {
            m_timer = std::make_shared<time_trigger::TimeTrigger>(SUMMER_ON, SUMMER_ON + ON_DURATION, m_gpio,
                                                                  "Summer fresh air", m_sysLogger);
        }
        m_oldShouldWarmup = isShouldWarm;
    }
}
}
