#include "RotiController.hpp"
#include <Constants.hpp>
#include <SysLogger.hpp>

#include <cassert>
#include <sstream>

namespace controller
{

static const size_t LOG_INTERVALL = 30 * MIN_TO_SEC / CALL_INTERVALL_ROTI;

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
    bool increaseIndoor = set > indoor;
    bool isOutdoorHigher = indoor < outdoor;

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
    sensor::SensorData indoor = m_indoorSensor->getData();
    sensor::SensorData outdoor = m_outdoorSensor->getData();
    if (INVALID_FLOAT == indoor.temperature) {
        // Invalid sensor data
        m_sysLogger->logError(m_loggerId, "Invalid indoor value");
        return;
    } else if (INVALID_FLOAT == outdoor.temperature) {
        // Invalid sensor data
        m_sysLogger->logError(m_loggerId, "Invalid outdoor value");
        return;
    } else {
        // calculate absHum
        const float absHumIndoor = relHumidityToAbs(indoor.temperature, indoor.humidity);
        const float absHumOutdoor = relHumidityToAbs(outdoor.temperature, outdoor.humidity);
        const float absHumSet = relHumidityToAbs(SET_TEMP, SET_HUM);

        static size_t counter = 0;
        if (0 == counter) {
            std::stringstream logSs;
            logSs << "AbsHumIndoor: " << absHumIndoor << "\tAbsHumOutdoor: " << absHumOutdoor
                  << "\tAbsHumSet: " << absHumSet;
            m_sysLogger->logMsg(m_loggerId, logSs.str());
            ++counter;
            counter %= LOG_INTERVALL;
        }

        // set roti output
        if (shouldBeEnabled(absHumIndoor, absHumOutdoor, absHumSet)) {
            outputValue = gpio::Value::HIGH;
        }
    }
    m_gpioRoti->setValue(m_loggerId, outputValue);
    m_sysLogger->logOutput(m_loggerId, outputValue);
}
}
