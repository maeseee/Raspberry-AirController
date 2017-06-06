#include "GpioOr.hpp"
#include <Controller/SensorController.hpp>
#include <SysLogger.hpp>
#include <Utility/Constants.hpp>

#include <algorithm>
#include <cassert>
#include <sstream>

namespace gpio
{

GpioOr::GpioOr(const IGpioPtr& gpioOutput,
               const controller::SensorControllerPtr& sensController,
               const logger::SysLoggerPtr& sysLogger)
    : m_gpioOutput(gpioOutput)
    , m_sensController(sensController)
    , m_sysLogger(sysLogger)
    , m_loggerId(sysLogger->generateId("GpioOr"))
{
    assert(m_gpioOutput);
    assert(m_sensController);
    assert(m_sysLogger);
}

bool GpioOr::setDirection(const size_t /*controllerId*/, const Direction dir)
{
    // Direction can only be out for the GpioCollection
    assert(Direction::OUT == dir);
    return true;
}

Direction GpioOr::getDirection() const
{
    return m_gpioOutput->getDirection();
}

bool GpioOr::setValue(const size_t id, const Value val)
{
    if (Value::HIGH == val) {
        m_controllerIdHigh.insert(id);
    } else {
        m_controllerIdHigh.erase(id);
    }

    const Value aimSystemState = getValue();
    if (aimSystemState != m_lastSetValue) {
        m_lastSetValue = aimSystemState;

        // Only log on system change
        std::stringstream logSs;
        logSs << "Turning system ";
        if (Value::HIGH == aimSystemState) {
            logSs << "on because of ";
            for (const size_t taskId : m_controllerIdHigh) {
                logSs << m_sysLogger->getNameFromId(taskId);
            }
        } else {
            logSs << "off";
        }
        m_sysLogger->logMsg(m_loggerId, logSs.str());
    }
    m_gpioOutput->setValue(id, aimSystemState);

    return true;
}

Value GpioOr::getValue() const
{
    Value aimSystemState;
    if (m_controllerIdHigh.size() > 0) {
        aimSystemState = Value::HIGH;
    } else {
        aimSystemState = Value::LOW;
    }

    // Don't switch on durring the hot daytime

    if ((m_sensController->difIndoorTemperatur2Outdoor() < 0) && (m_sensController->difIndoorTemperatur2Set() > 0) &&
        (Value::HIGH == aimSystemState)) {
        const size_t daytime = getDaytime();

        const size_t startHotDay = 10 * HOUR_TO_SEC;
        const size_t endHotDay = 20 * HOUR_TO_SEC;
        if ((daytime > startHotDay) && (daytime < endHotDay)) {
            // It is durring the hot day time. Switch of the air
            aimSystemState = Value::LOW;
            m_sysLogger->logMsg(m_loggerId, "GpioOr set to off because of the heat although it should be on");
        }
    }

    return aimSystemState;
}

size_t GpioOr::getPinNumber() const
{
    return m_gpioOutput->getPinNumber();
}

} // gpio
