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

bool TempController::shouldWarm()
{
    time_t t = time(NULL); // get time now
    struct tm* now = localtime(&t);

    // Example of date of today
    //  std::cout << "Date of today is: " << (now->tm_year + 1900) << '-'
    //            << (now->tm_mon + 1) << '-' << now->tm_mday << std::endl;

    const size_t month = now->tm_mon + 1;
    const size_t day = now->tm_mday;
    const float month_fp = static_cast<float>(month) + static_cast<float>(day) / 30.0;
    if ((month_fp > 5.5) && (month_fp < 9.0)) {
        // Cool in the months July and August
        return false;
    } else {
        return true;
    }
}
}
