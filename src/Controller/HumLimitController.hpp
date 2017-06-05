#pragma once

#include <Gpio/IGpio.hpp>
#include <Sensor/ISensor.hpp>
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

namespace controller
{
class SensorController;
using SensorControllerPtr = std::shared_ptr<SensorController>;

// Class
class HumLimitController : public threading::Threading
{
public:
    /**
   * @brief TemperatureController turns on and off the air system
   * @param gpioMainSystem gpio for switching on and off
   */
    HumLimitController(const sensor::SensorDataCPtr& indoorSensor,
                       const sensor::SensorDataCPtr& outdoorSensor,
                       const gpio::IGpioPtr& gpioMainSystem,
                       const logger::SysLoggerPtr& sysLogger);

    void recall() override;

private:
    bool shouldWarm() const;

    sensor::SensorDataCPtr m_indoorSensor;
    sensor::SensorDataCPtr m_outdoorSensor;
    gpio::IGpioPtr m_gpio;

    time_trigger::TimeTriggerPtr m_timer;

    const logger::SysLoggerPtr m_sysLogger;
    const size_t m_loggerId;
};
}
