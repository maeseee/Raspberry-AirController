#pragma once

#include <Gpio/IGpio.hpp>

#include <memory>
#include <set>

// FWD
namespace controller
{
class SensorController;
using SensorControllerPtr = std::shared_ptr<SensorController>;
} // namespace controller

namespace logger
{
class SysLogger;
using SysLoggerPtr = std::shared_ptr<SysLogger>;
} // namespace logger

// class
namespace gpio
{

/**
 * @brief The GpioOr class checks if at least one controller say turnung on
 * IMPORTANT. The destructor of the controller should set the value to
 * Value::LOW
 */
class GpioOr : public IGpio
{
public:
    explicit GpioOr(const IGpioPtr& gpioOutput,
                    const controller::SensorControllerPtr& sensController,
                    const logger::SysLoggerPtr& sysLogger);

    bool setDirection(const size_t controllerId, const Direction dir) override;
    Direction getDirection() const override;

    bool setValue(const size_t controllerId, const Value val) override;
    Value getValue() const override;

    size_t getPinNumber() const override;

private:
    const IGpioPtr m_gpioOutput;
    const controller::SensorControllerPtr m_sensController;

    std::set<size_t> m_controllerIdHigh;

    Value m_lastSetValue{Value::INVALID}; // Used because of threading problems
    const logger::SysLoggerPtr m_sysLogger;
    const size_t m_loggerId;
};
using GpioOrPtr = std::shared_ptr<GpioOr>;
} // namespace gpio
