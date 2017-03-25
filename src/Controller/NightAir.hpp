#pragma once

#include <Gpio/IGpio.hpp>

#include <vector>

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

class NightAir
{
public:
    NightAir(const gpio::IGpioPtr& gpio, const logger::SysLoggerPtr& sysLogger);

    gpio::Value getValue() const;

private:
    void addTimer(size_t onTime);

    gpio::IGpioPtr m_gpio;
    std::vector<time_trigger::TimeTriggerPtr> m_timers;

    const logger::SysLoggerPtr m_sysLogger;
};
}
