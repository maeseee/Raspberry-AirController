#include "OneTimeTrigger.hpp"
#include <Gpio/IGpio.hpp>
#include <SysLogger.hpp>

#include <unistd.h> // for sleep

namespace time_trigger
{

time_trigger::OneTimeTrigger::OneTimeTrigger(const gpio::IGpioPtr& gpio, const logger::SysLoggerPtr& sysLogger)
    : m_gpio(gpio)
    , m_sysLogger(sysLogger)
{
}

time_trigger::OneTimeTrigger::~OneTimeTrigger()
{
    m_stopThread = true;
}

void OneTimeTrigger::addTrigger(const size_t duration)
{
    m_stopThread = true;
    if (m_thread.joinable()) {
        m_thread.join();
    }

    m_stopThread = false;
    m_thread = std::thread(&OneTimeTrigger::threadFn, this, duration);
}

void OneTimeTrigger::threadFn(size_t duration)
{
    size_t loggerId = m_sysLogger->generateId("OneTimeTrigger");
    m_gpio->setValue(loggerId, gpio::Value::HIGH);

    for (size_t durationCounter = 0; (false == m_stopThread) && (durationCounter < duration); ++durationCounter) {
        sleep(1);
    }

    m_gpio->setValue(loggerId, gpio::Value::LOW);
}
}
