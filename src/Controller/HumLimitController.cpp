#include "HumLimitController.hpp"
#include <Gpio/IGpio.hpp>
#include <SysLogger.hpp>
#include <Utility/Constants.hpp>

#include <cassert>
#include <cmath>
#include <ctime>
#include <sstream>

namespace controller
{

HumLimitController::HumLimitController(const sensor::SensorDataCPtr& indoorSensor,
                                       const sensor::SensorDataCPtr& outdoorSensor,
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
    // calculate absHum
    const float absHumIndoor = relHumidityToAbs(m_indoorSensor->m_temperature, m_indoorSensor->m_humidity);
    const float absHumOutdoor = relHumidityToAbs(m_outdoorSensor->m_temperature, m_outdoorSensor->m_humidity);
    const float absHumUpperLimit = relHumidityToAbs(m_indoorSensor->m_temperature, SET_HUM + HUM_LIMIT_TOLERANCE);
    const float absHumLowerLimit = relHumidityToAbs(m_indoorSensor->m_temperature, SET_HUM - HUM_LIMIT_TOLERANCE);

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
