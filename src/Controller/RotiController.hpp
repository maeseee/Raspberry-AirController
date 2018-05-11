#pragma once

#include <Threading.hpp>

// FWD
namespace logger
{
class SysLogger;
using SysLoggerPtr = std::shared_ptr<SysLogger>;
}

namespace gpio
{
class GpioCollector;
using GpioCollectorPtr = std::shared_ptr<GpioCollector>;

class IGpio;
using IGpioPtr = std::shared_ptr<IGpio>;

enum class Value;
}

namespace controller
{
class SensorController;
using SensorControllerPtr = std::shared_ptr<SensorController>;
}

// Class
namespace controller
{

class RotiController : public threading::Threading
{
public:
    /**
   * @brief RotiController controlls the humidity in the room
   * @param indoorSensor sensor for indoor humidity [%]
   * @param outdoorSensor sensor for outdoor humidity [%]
   * @param gpioRoti gpio for controlling the roti
   */
    RotiController(const controller::SensorControllerPtr sensController,
                   const gpio::IGpioPtr& gpioRoti,
                   const logger::SysLoggerPtr& sysLogger);

    ~RotiController();

    void recall() override;

private:
    bool shouldBeEnabled(const double indoor, const double outdoor, const double set) const;

    const controller::SensorControllerPtr m_sensController;
    const gpio::IGpioPtr m_gpioRoti;

    gpio::Value m_lastOutputValue;

    const logger::SysLoggerPtr m_sysLogger;
    const size_t m_loggerId;
};
}
