#include "SensorController.hpp"
#include <Sensor/ISensor.hpp>
#include <SysLogger.hpp>
#include <Utility/Constants.hpp>

#include <cassert>
#include <sstream>

namespace controller
{

static const size_t LOG_INTERVALL = 1 * HOUR_TO_SEC / CALL_INTERVALL_ROTI;

SensorController::SensorController(const sensor::SensorDataCPtr& indoorSensor,
                                   const sensor::SensorDataCPtr& outdoorSensor)
    : m_indoorSensor(indoorSensor)
    , m_outdoorSensor(outdoorSensor)
{
    assert(m_indoorSensor);
    assert(m_outdoorSensor);
}

float SensorController::difIndoorTemperatur2Outdoor() const
{
    return m_indoorSensor->m_temperature - m_outdoorSensor->m_temperature;
}

float SensorController::difIndoorHumidity2Outdoor() const
{
    const float absHumIndoor = relHumidityToAbs(m_indoorSensor->m_temperature, m_indoorSensor->m_humidity);
    const float absHumOutdoor = relHumidityToAbs(m_outdoorSensor->m_temperature, m_outdoorSensor->m_humidity);

    return absHumIndoor - absHumOutdoor;
}

float SensorController::difIndoorTemperatur2Set(const float tempSet) const
{
    return m_indoorSensor->m_temperature - tempSet;
}

float SensorController::difIndoorHumidity2Set(const float humiditySet) const
{
    const float absHumIndoor = relHumidityToAbs(m_indoorSensor->m_temperature, m_indoorSensor->m_humidity);
    const float absHumSet = relHumidityToAbs(m_indoorSensor->m_temperature, humiditySet);
    return absHumIndoor - absHumSet;
}

bool SensorController::shouldRotiBeEnabled(const float humiditySet) const
{
    const bool isSetHigherThanIndoor = difIndoorHumidity2Set(humiditySet) > 0 ? false : true;
    const bool isOutdoorHigherThanIndoor = difIndoorHumidity2Outdoor() > 0 ? false : true;

    // SetLowerThanIndoor   OutdoorLowerThanIndoor   -> off
    // SetLowerThanIndoor   OutdoorHigherThanIndoor  -> on
    // SetHigherThanIndoor  OutdoorLowerThanIndoor   -> on
    // SetHigherThanIndoor  OutdoorHigherThanIndoor  -> off
    return isSetHigherThanIndoor ^ isOutdoorHigherThanIndoor;
}

bool SensorController::shouldHumControllerBeEnabled() const
{
    return !shouldRotiBeEnabled(SET_HUM + 5.0);
}
}
