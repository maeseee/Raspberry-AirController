#pragma once

#include <Gpio/IGpio.hpp>
#include <Sensor/ISensor.hpp>
#include <Threading.hpp>

// FWD
namespace logger
{
class SysLogger;
using SysLoggerPtr = std::shared_ptr<SysLogger>;
} // namespace logger

// Class
namespace sensor
{

class Am2302Sensor : public sensor::ISensor, public threading::Threading
{
public:
    Am2302Sensor(const gpio::IGpioPtr& sensor, const logger::SysLoggerPtr& sysLogger);

    SensorDataCPtr getData() const override;

    void recall() override;

private:
    int waitForBit(const gpio::Value val) const;
    int readBit() const;
    int readByte() const;

    SensorDataPtr m_data;

    gpio::IGpioPtr m_sensor;

    const logger::SysLoggerPtr m_sysLogger;
    const size_t m_loggerIdTemp{0};
    const size_t m_loggerIdHum{0};
};
} // namespace sensor
