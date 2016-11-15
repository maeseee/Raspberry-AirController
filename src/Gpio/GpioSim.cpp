#include "GpioSim.hpp"
#include <SysLogger.hpp>

namespace gpio {

GpioSim::GpioSim(const std::string &name) : m_name(name) {

  logger::SysLogger::instance().log("GPIO " + name + " is simuluted!");
}

bool GpioSim::setDirection(const size_t /*controllerId*/, const Direction dir) {
  m_dir = dir;
  return true;
}

Direction GpioSim::getDirection() const { return m_dir; }

bool GpioSim::setValue(const size_t /*controllerId*/, const Value val) {
  // only set value if it is an output
  if (Direction::OUT != m_dir) {
    return false;
  }

  std::string valueString = ((val == Value::HIGH) ? "1" : "0");
  logger::SysLogger::instance().log("GPIO " + m_name + " set to " +
                                    valueString);

  m_val = val;
  return true;
}

Value GpioSim::getValue() const { return m_val; }

size_t gpio::GpioSim::getPinNumber() const { return 0; }

} // gpio
