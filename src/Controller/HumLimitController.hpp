#pragma once

#include <Threading.hpp>

// FWD
namespace logger
{
class SysLogger;
using SysLoggerPtr = std::shared_ptr<SysLogger>;
} // namespace logger

namespace gpio
{
class IGpio;
using IGpioPtr = std::shared_ptr<IGpio>;
} // namespace gpio

namespace time_trigger
{
class TimeTrigger;
using TimeTriggerPtr = std::shared_ptr<TimeTrigger>;
} // namespace time_trigger

namespace controller
{
class SensorController;
using SensorControllerPtr = std::shared_ptr<SensorController>;
} // namespace controller

// Class
namespace controller
{

class HumLimitController : public threading::Threading
{
public:
    /**
     * @brief TemperatureController turns on and off the air system
     * @param gpioMainSystem gpio for switching on and off
     */
    HumLimitController(const SensorControllerPtr& sensController,
                       const gpio::IGpioPtr& gpioMainSystem,
                       const logger::SysLoggerPtr& sysLogger);

    void recall() override;

private:
    const controller::SensorControllerPtr m_sensController;
    const gpio::IGpioPtr m_gpio;

    time_trigger::TimeTriggerPtr m_timer;

    const logger::SysLoggerPtr m_sysLogger;
    const size_t m_loggerId;
};
} // namespace controller
