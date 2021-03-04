#include "RotiController.hpp"
#include <Controller/SensorController.hpp>
#include <Gpio/IGpio.hpp>
#include <Sensor/ISensor.hpp>
#include <SysLogger.hpp>
#include <Utility/Constants.hpp>

#include <cassert>
#include <sstream>

namespace controller
{

RotiController::RotiController(const controller::SensorControllerPtr sensController,
                               const gpio::IGpioPtr& gpioRoti,
                               const logger::SysLoggerPtr& sysLogger)
    : threading::Threading(CALL_INTERVALL_ROTI)
    , m_sensController(sensController)
    , m_gpioRoti(gpioRoti)
    , m_lastOutputValue(gpio::Value::INVALID)
    , m_sysLogger(sysLogger)
    , m_loggerId(sysLogger->generateId("RotiController"))
{
    assert(m_sensController);
    assert(m_gpioRoti);

    setInitialized();
}

RotiController::~RotiController()
{
    m_gpioRoti->setValue(m_loggerId, gpio::Value::LOW);
}

bool RotiController::shouldBeEnabled(const double indoor, const double outdoor, const double set) const
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

    // set roti output
    if (m_sensController->shouldRotiBeEnabled(SET_HUM)) {
        outputValue = gpio::Value::HIGH;
    }

    m_gpioRoti->setValue(m_loggerId, outputValue);

    if (outputValue != m_lastOutputValue) {
        m_sysLogger->logOutput(m_loggerId, outputValue);
        m_lastOutputValue = outputValue;
    }
}
} // namespace controller
