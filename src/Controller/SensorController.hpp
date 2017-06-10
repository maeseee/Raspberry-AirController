#pragma once

#include <memory>

// FWD
namespace logger
{
class SysLogger;
using SysLoggerPtr = std::shared_ptr<SysLogger>;
}

namespace sensor
{
class SensorData;
using SensorDataCPtr = std::shared_ptr<const SensorData>;
}

// Class
namespace controller
{

class SensorController
{
public:
    /**
   * @brief RotiController controlls the humidity in the room
   * @param indoorSensor sensor for indoor humidity [%]
   * @param outdoorSensor sensor for outdoor humidity [%]
   * @param gpioRoti gpio for controlling the roti
   */
    SensorController(const sensor::SensorDataCPtr& indoorSensor, const sensor::SensorDataCPtr& outdoorSensor);

    ~SensorController() = default;

    /**
     * @brief difIndoorTemperatur2Outdoor (Indoor - Outdoor)
     * @return difference [K]
     */
    float difIndoorTemperatur2Outdoor() const;

    /**
     * @brief difIndoorHumidity2Outdoor (Indoor - Outdoor)
     * @return difference [g/m3]
     */
    float difIndoorHumidity2Outdoor() const;

    /**
     * @brief difIndoorTemperatur2Set (Indoor - Set)
     * @return difference [K]
     */
    float difIndoorTemperatur2Set(const float tempSet) const;

    /**
     * @brief difIndoorHumidity2Set (IndoorAbs - SetAbs)
     * @param humiditySet [%]
     * @return difference [g/m3]
     */
    float difIndoorHumidity2Set(const float humiditySet) const;

    bool shouldRotiBeEnabled(const float humiditySet) const;
    bool shouldHumControllerBeEnabled() const;

private:
    float relHumidityToAbs(const float tempC, const float humidityRel) const;
    float absHumidityToRel(const float tempC, const float humidityAbs) const;

    const sensor::SensorDataCPtr m_indoorSensor;
    const sensor::SensorDataCPtr m_outdoorSensor;
};
using SensorControllerPtr = std::shared_ptr<SensorController>;
}
