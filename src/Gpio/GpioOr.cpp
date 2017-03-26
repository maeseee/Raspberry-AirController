#include "GpioOr.hpp"
#include <SysLogger.hpp>

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
    const Value systemState = getValue();
    std::stringstream logSs;
    logSs << "Turning system " << (Value::HIGH == systemState ? "on" : "off");
    m_sysLogger->logMsg(m_loggerId, logSs.str());
    m_gpioOutput->setValue(currentId, systemState);

    return true;
}

Value GpioOr::getValue() const
{
    if (m_controllerIdHigh.size() > 0) {
        return Value::HIGH;
    } else {
        return Value::LOW;
    }
}

size_t GpioOr::getPinNumber() const
{
    return m_gpioOutput->getPinNumber();
}

} // gpio
