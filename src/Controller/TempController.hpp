#pragma once

#include <Gpio/IGpio.hpp>
#include <Threading.hpp>

// FWD
namespace logger
{
class SysLogger;
using SysLoggerPtr = std::shared_ptr<SysLogger>;
}

namespace time_trigger
{
class TimeTrigger;
using TimeTriggerPtr = std::shared_ptr<TimeTrigger>;
}

// Class
namespace controller
{

class TempController : public threading::Threading
{
public:
    /**
   * @brief TemperatureController turns on and off the air system
   * @param gpioMainSystem gpio for switching on and off
   */
    TempController(const gpio::IGpioPtr& gpioMainSystem, const logger::SysLoggerPtr& sysLogger);

    void recall() override;

private:
    bool shouldWarm() const;

    gpio::IGpioPtr m_gpio;
    time_trigger::TimeTriggerPtr m_timer;
    bool m_oldShouldWarmup{true};

    const logger::SysLoggerPtr m_sysLogger;
    const size_t m_loggerId;
};
}
