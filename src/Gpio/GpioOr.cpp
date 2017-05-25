#include "GpioOr.hpp"
#include <SysLogger.hpp>
#include <Utility/Constants.hpp>

#include <algorithm>
#include <cassert>
#include <sstream>

namespace gpio
{

GpioOr::GpioOr(const IGpioPtr& gpioOutput, const logger::SysLoggerPtr& sysLogger)
    : m_gpioOutput(gpioOutput)
    , m_sysLogger(sysLogger)
    , m_loggerId(sysLogger->generateId("GpioOr"))
{
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
        m_controllerIdHigh.push_back(id);
    } else {
        m_controllerIdHigh.erase(std::remove(m_controllerIdHigh.begin(), m_controllerIdHigh.end(), id),
                                 m_controllerIdHigh.end());
    }

    size_t currentId = id;
    if (Value::HIGH == getValue()) {
        currentId = m_controllerIdHigh.back();
    }

    const Value aimSystemState = getValue();
    if (aimSystemState != m_lastSetValue) {
        m_lastSetValue = aimSystemState;

        // Only log on system change
        std::stringstream logSs;
        logSs << "Turning system ";
        if (Value::HIGH == aimSystemState) {
            logSs << "on because of ";
            for (const size_t id : m_controllerIdHigh) {
                logSs << m_sysLogger->getNameFromId(id) << " (" << id << "),";
            }
        } else {
            logSs << "off";
        }
        m_sysLogger->logMsg(m_loggerId, logSs.str());
    }
    m_gpioOutput->setValue(currentId, aimSystemState);

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
    if (not shouldWarm()) {
        const size_t daytime = getDaytime();

        bool isHotDayTime = false;
        const size_t startHotDay = 10 * HOUR_TO_SEC;
        const size_t endHotDay = 20 * HOUR_TO_SEC;
        if ((daytime > startHotDay) && (daytime < endHotDay)) {
            // It is durring the hot day time. Switch of the air
            aimSystemState = Value::LOW;
        }
    }

    return aimSystemState;
}

size_t GpioOr::getPinNumber() const
{
    return m_gpioOutput->getPinNumber();
}

} // gpio
