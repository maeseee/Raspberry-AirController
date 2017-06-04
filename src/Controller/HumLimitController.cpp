#include "HumLimitController.hpp"
#include <SysLogger.hpp>
#include <Utility/Constants.hpp>

#include <cassert>
#include <cmath>
#include <ctime>
#include <sstream>

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
    , m_loggerId(sysLogger->generateId("HumLimitController"))
{
    assert(m_indoorSensor);
    assert(m_outdoorSensor);
    assert(m_gpio);

    setInitialized();
}

void HumLimitController::recall()
{
    // grap sensor values
    const sensor::SensorDataPtr indoor = m_indoorSensor->getData();
    const sensor::SensorDataPtr outdoor = m_outdoorSensor->getData();
    if ((nullptr == indoor) || (nullptr == outdoor)) {
        m_sysLogger->logError(m_loggerId, "Invalid sensor value");
        return;
    }

    // calculate absHum
    const float absHumIndoor = relHumidityToAbs(indoor->m_temperature, indoor->m_humidity);
    const float absHumOutdoor = relHumidityToAbs(outdoor->m_temperature, outdoor->m_humidity);
    const float absHumUpperLimit = relHumidityToAbs(indoor->m_temperature, SET_HUM + HUM_LIMIT_TOLERANCE);
    const float absHumLowerLimit = relHumidityToAbs(indoor->m_temperature, SET_HUM - HUM_LIMIT_TOLERANCE);

    std::stringstream logSs;
    logSs << "AbsHumIndoor is " << absHumIndoor << " and AbsHumOutdoor is " << absHumOutdoor << ": ";

    if ((absHumUpperLimit < absHumIndoor) && (absHumOutdoor < absHumIndoor)) {
        // turn on the air controller to lower humidity
        m_gpio->setValue(m_loggerId, gpio::Value::HIGH);
        logSs << "The upper limit is " << absHumUpperLimit << " -> So fresh air is used to lower the indoor humidity";
    } else if ((absHumLowerLimit > absHumIndoor) && (absHumOutdoor > absHumIndoor)) {
        // turn on the air controller to higher humidity
        m_gpio->setValue(m_loggerId, gpio::Value::HIGH);
        logSs << "The lower limit is " << absHumLowerLimit << " -> So fresh air is used to higher the indoor humidity";
    } else {
        // turn off the air controller. Turning on does not make a better situation
        m_gpio->setValue(m_loggerId, gpio::Value::LOW);
        logSs << "Lower limit is " << absHumLowerLimit << " and upper limit is " << absHumUpperLimit
              << " -> So humidity is in range";
    }
    m_sysLogger->logMsg(m_loggerId, logSs.str());
}
}
