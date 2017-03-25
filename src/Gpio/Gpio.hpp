#pragma once

#include <Gpio/GpioConstants.hpp>
#include <Gpio/IGpio.hpp>

// FWD
namespace logger
{
class SysLogger;
using SysLoggerPtr = std::shared_ptr<SysLogger>;
}

// Class
namespace gpio
{

/**
 * @brief The Gpio class controls one GPIO pin
 */
class Gpio : public IGpio
{
public:
    explicit Gpio(const Function function, const Direction dir, const Value val, const logger::SysLoggerPtr& sysLogger);

    ~Gpio();

    bool setDirection(const size_t controllerId, const Direction dir) override;
    Direction getDirection() const override;

    bool setValue(const size_t loggerId, const Value val) override;
    Value getValue() const override;

    size_t getPinNumber() const override;

private:
    /**
     * @brief export_gpio Export the GPIO
     * @return true for successful
     */
    bool exportGpio();

    /**
     * @brief unexport_gpio Unexport the GPIO
     * @return true for succesful
     */
    bool unexportGpio();

    size_t m_gpioNumber; // GPIO number associated with the instance of an object

    const logger::SysLoggerPtr m_sysLogger;
    size_t m_loggerId{0};
};
}
