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

// Class
namespace gpio
{
class GpioCollector;
using GpioCollectorPtr = std::shared_ptr<GpioCollector>;
}
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
    RotiController(const sensor::ISensorPtr& indoorSensor,
                   const sensor::ISensorPtr& outdoorSensor,
                   const gpio::IGpioPtr& gpioRoti,
                   const logger::SysLoggerPtr& sysLogger);

    ~RotiController();

    void recall() override;

private:
    bool shouldBeEnabled(const float indoor, const float outdoor, const float set) const;

    sensor::ISensorPtr m_indoorSensor;
    sensor::ISensorPtr m_outdoorSensor;
    gpio::IGpioPtr m_gpioRoti;

    float m_measuredTempIndoor;  // [°C]
    float m_measuredTempOutdoor; // [°C]

    float m_measuredHumIndoor;  // [%]
    float m_measuredHumOutdoor; // [%]

    const logger::SysLoggerPtr m_sysLogger;
    size_t m_loggerId{0};
};
}
