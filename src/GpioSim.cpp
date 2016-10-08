#include "GpioSim.hpp"

#include <iostream>

namespace gpio {

GpioSim::GpioSim(const std::string &name) : m_name(name) {
  std::cout << "GPIO " << name << " is simuluted!" << std::endl;
}

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

  std::cout << "Set value " << static_cast<int>(val) << " on " << m_name
            << std::endl;
  m_val = val;
  return true;
}

Value GpioSim::getValue() const { return m_val; }

} // gpio
