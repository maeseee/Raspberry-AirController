#include "RotiController.hpp"
#include <SysLogger.hpp>
#include <Utility/Constants.hpp>

#include <cassert>
#include <sstream>

namespace controller
{

static const size_t LOG_INTERVALL = 1 * HOUR_TO_SEC / CALL_INTERVALL_ROTI;

RotiController::RotiController(const sensor::ISensorPtr& indoorSensor,
                               const sensor::ISensorPtr& outdoorSensor,
                               const gpio::IGpioPtr& gpioRoti,
                               const logger::SysLoggerPtr& sysLogger)
    : threading::Threading(CALL_INTERVALL_ROTI)
    , m_indoorSensor(indoorSensor)
    , m_outdoorSensor(outdoorSensor)
    , m_gpioRoti(gpioRoti)
    , m_sysLogger(sysLogger)
    , m_loggerId(sysLogger->generateId("RotiController"))
{
    assert(m_indoorSensor);
    assert(m_outdoorSensor);
    assert(m_gpioRoti);

    setInitialized();
}

RotiController::~RotiController()
{
    m_gpioRoti->setValue(m_loggerId, gpio::Value::LOW);
}

bool RotiController::shouldBeEnabled(const float indoor, const float outdoor, const float set) const
{
    const bool increaseIndoor = set > indoor;
    const bool isOutdoorHigher = indoor < outdoor;

    // decreaseIndoor  OutdoorLower   -> off
    // decreaseIndoor  OutdoorHigher  -> on
    // increaseIndoor  OutdoorLower  -> on
    // increaseIndoor  OutdoorHigher  -> off
    return increaseIndoor ^ isOutdoorHigher;
}

void RotiController::recall()
{
    gpio::Value outputValue = gpio::Value::LOW;

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
    const float absHumSet = relHumidityToAbs(SET_TEMP, SET_HUM);

    // add some sensor information in the log file
    if (0 == m_logCounter) {
        std::stringstream logSs;
        logSs << "AbsHumIndoor: " << absHumIndoor << "\tAbsHumOutdoor: " << absHumOutdoor
              << "\tAbsHumSet: " << absHumSet;
        m_sysLogger->logMsg(m_loggerId, logSs.str());
        ++m_logCounter;
        m_logCounter %= LOG_INTERVALL;
    }

    // set roti output
    if (shouldBeEnabled(absHumIndoor, absHumOutdoor, absHumSet)) {
        outputValue = gpio::Value::HIGH;
    }

    m_gpioRoti->setValue(m_loggerId, outputValue);

    if (outputValue != m_lastOutputValue) {
        m_sysLogger->logOutput(m_loggerId, outputValue);
        m_lastOutputValue = outputValue;
    }
}
}
