#include "GpioSim.hpp"

namespace gpio {

GpioSim::GpioSim() {}

bool GpioSim::exportGpio() {}

bool GpioSim::unexportGpio() {}

bool GpioSim::setDirection(const Direction dir) {
  m_dir = dir;
  return true;
}

Direction GpioSim::getDirection() const { return m_dir; }

bool GpioSim::setValue(const Value val) {
  // only set value if it is an output
  if (Direction::OUT != m_dir) {
    return false;
  }

  m_val = val;
  return true;
}

Value GpioSim::getValue() const { return m_val; }

} // gpio
