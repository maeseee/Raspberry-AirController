#pragma once

#include <thread>

// FWD
namespace logger
{
class SysLogger;
using SysLoggerPtr = std::shared_ptr<SysLogger>;
}

namespace gpio
{
class IGpio;
using IGpioPtr = std::shared_ptr<IGpio>;
}

// Class
namespace time_trigger
{

class OneTimeTrigger
{
public:
    OneTimeTrigger(const gpio::IGpioPtr& gpio, const logger::SysLoggerPtr& sysLogger);
    ~OneTimeTrigger();

    void addTrigger(const size_t duration);

private:
    void threadFn(size_t duration);

    gpio::IGpioPtr m_gpio;

    const logger::SysLoggerPtr m_sysLogger;

    std::thread m_thread;
    bool m_stopThread{false};
};

using OneTimeTriggerPtr = std::shared_ptr<OneTimeTrigger>;
}
