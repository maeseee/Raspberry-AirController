#pragma once

#include <Gpio/IGpio.hpp>

#include <string>

// FWD
namespace logger
{
class SysLogger;
using SysLoggerPtr = std::shared_ptr<SysLogger>;
} // namespace logger

// Class
namespace gpio
{

/**
 * @brief The Gpio class controls one GPIO pin
 */
class GpioSim : public IGpio
{
public:
    explicit GpioSim(const Function function, const logger::SysLoggerPtr& sysLogger);

    bool setDirection(const size_t controllerId, const Direction dir) override;
    Direction getDirection() const override;

    bool setValue(const size_t id, const Value val) override;
    Value getValue() const override;

    size_t getPinNumber() const override;

private:
    Direction m_dir{Direction::UNSET};
    Value m_val{Value::INVALID};

    const logger::SysLoggerPtr m_sysLogger;
    const size_t m_loggerId;
};
} // namespace gpio
