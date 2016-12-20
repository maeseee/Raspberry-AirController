#include "GpioSim.hpp"
#include <SysLogger.hpp>

#include <iostream>
namespace gpio {

GpioSim::GpioSim(const Function function, const logger::SysLoggerPtr &sysLogger)
    : m_sysLogger(sysLogger) {

  m_loggerId = m_sysLogger->generateId("GpioSim");
  m_sysLogger->logMsg(
      m_loggerId, "GPIO for " + std::to_string(static_cast<size_t>(function)) +
                      " is simulated!");
}

bool GpioSim::setDirection(const size_t /*controllerId*/, const Direction dir) {
  m_dir = dir;
  return true;
}

Direction GpioSim::getDirection() const { return m_dir; }

bool GpioSim::setValue(const size_t id, const Value val) {
  // only set value if it is an output
  if (Direction::OUT != m_dir) {
    return false;
  }

  std::string valueString = ((val == Value::HIGH) ? "1" : "0");

  m_sysLogger->logOutput(id, val);

  m_val = val;
  return true;
}

Value GpioSim::getValue() const { return m_val; }

size_t gpio::GpioSim::getPinNumber() const { return 0; }

} // gpio
