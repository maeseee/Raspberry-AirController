#include "HumLimitController.hpp"
#include <Constants.hpp>
#include <SysLogger.hpp>

#include <cassert>
#include <cmath>
#include <ctime>

namespace controller
{

HumLimitController::HumLimitController(const sensor::ISensorPtr& indoorSensor,
                                       const sensor::ISensorPtr& outdoorSensor,
                                       const gpio::IGpioPtr& gpioMainSystem,
                                       const logger::SysLoggerPtr& sysLogger)
    : threading::Threading(CALL_INTERVALL_HUMLIMIT)
    , m_indoorSensor(indoorSensor)
    , m_outdoorSensor(outdoorSensor)
    , m_gpio(gpioMainSystem)
    , m_sysLogger(sysLogger)
{
    assert(m_indoorSensor);
    assert(m_outdoorSensor);
    assert(m_gpio);

    m_loggerId = m_sysLogger->generateId("HumLimitController");
}

void HumLimitController::recall()
{

    const sensor::SensorData indoorData = m_indoorSensor->getData();
    const sensor::SensorData outdoorData = m_outdoorSensor->getData();

    // calculate absHum
    const float absHumIndoor = relHumidityToAbs(indoorData.temperature, indoorData.humidity);
    const float absHumOutdoor = relHumidityToAbs(outdoorData.temperature, outdoorData.humidity);
    const float absHumUpperLimit = relHumidityToAbs(SET_TEMP, SET_HUM + HUM_LIMIT_TOLERANCE);
    const float absHumLowerLimit = relHumidityToAbs(SET_TEMP, SET_HUM - HUM_LIMIT_TOLERANCE);

    if ((absHumUpperLimit < absHumIndoor) && (absHumOutdoor < absHumIndoor)) {
        // turn on the air controller to lower humidity
        m_gpio->setValue(m_loggerId, gpio::Value::HIGH);
        m_sysLogger->logMsg(m_loggerId, "Fresh air can be used to lower the indoor humidity");
    } else if ((absHumLowerLimit > absHumIndoor) && (absHumOutdoor > absHumIndoor)) {
        // turn on the air controller to higher humidity
        m_gpio->setValue(m_loggerId, gpio::Value::HIGH);
        m_sysLogger->logMsg(m_loggerId, "Fresh air can be used to higher the indoor humidity");
    } else {
        // turn off the air controller. Turning on does not make a better situation
        m_gpio->setValue(m_loggerId, gpio::Value::LOW);
        m_sysLogger->logMsg(m_loggerId,
                            ("Humidity is in range: absHumIndoor(" + std::to_string(absHumIndoor) + ") absHumOutdoor(" +
                             std::to_string(absHumOutdoor) + ") absHumUpperLimit(" + std::to_string(absHumUpperLimit) +
                             ") absHumLowerLimit" + std::to_string(absHumLowerLimit) + ")"));
    }
}
}
