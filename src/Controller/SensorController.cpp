#include "SensorController.hpp"
#include <Sensor/ISensor.hpp>
#include <SysLogger.hpp>
#include <Utility/Constants.hpp>

#include <cassert>
#include <cmath>
#include <sstream>

namespace controller
{

SensorController::SensorController(const sensor::SensorDataCPtr& indoorSensor,
                                   const sensor::SensorDataCPtr& outdoorSensor)
    : m_indoorSensor(indoorSensor)
    , m_outdoorSensor(outdoorSensor)
{
    assert(m_indoorSensor);
    assert(m_outdoorSensor);
}

double SensorController::difIndoorTemperatur2Outdoor() const
{
    return m_indoorSensor->m_temperature - m_outdoorSensor->m_temperature;
}

double SensorController::difIndoorHumidity2Outdoor() const
{
    const double absHumIndoor = relHumidityToAbs(m_indoorSensor->m_temperature, m_indoorSensor->m_humidity);
    const double absHumOutdoor = relHumidityToAbs(m_outdoorSensor->m_temperature, m_outdoorSensor->m_humidity);

    return absHumIndoor - absHumOutdoor;
}

double SensorController::difIndoorTemperatur2Set(const double tempSet) const
{
    return m_indoorSensor->m_temperature - tempSet;
}

double SensorController::difIndoorHumidity2Set(const double humiditySet) const
{
    const double absHumIndoor = relHumidityToAbs(m_indoorSensor->m_temperature, m_indoorSensor->m_humidity);
    const double absHumSet = relHumidityToAbs(m_indoorSensor->m_temperature, humiditySet);
    return absHumIndoor - absHumSet;
}

bool SensorController::shouldRotiBeEnabled(const double humiditySet) const
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

double SensorController::relHumidityToAbs(const double tempC, const double humidityRel) const
{
    // function calculated for
    // Temperatur [°C]  water for 100 % [g/m3]
    // -20              0.9
    // -15	            1.4
    // -10	            2.1
    //  -5	            3.3
    //   0	            4.8
    //   5	            6.8
    //  10	            9.4
    //  15	           12.8
    //  20	           17.3
    //  25	           23.0
    //  30	           30.3
    //  35	           39.6
    //  40	           51.1

    return static_cast<double>(4.2431796244 * exp(0.0666427637 * tempC) * humidityRel / 100.0);
}

double SensorController::absHumidityToRel(const double tempC, const double humidityAbs) const
{
    const double absHumidity100 = relHumidityToAbs(tempC, 100);
    return humidityAbs / absHumidity100 * 100;
}
} // namespace controller
