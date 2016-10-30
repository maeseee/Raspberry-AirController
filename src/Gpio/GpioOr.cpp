#include "GpioOr.hpp"

#include <cassert>

namespace gpio {

GpioOr::GpioOr(const IGpioPtr &gpioOutput) : m_gpioOutput(gpioOutput) {}

bool GpioOr::setDirection(const size_t /*controllerId*/, const Direction dir) {
  // Direction can only be out for the GpioCollection
  assert(Direction::OUT == dir);
  return true;
}

Direction GpioOr::getDirection() const { return m_gpioOutput->getDirection(); }

bool GpioOr::setValue(const size_t id, const Value val) {
  m_valueMap[id] = val;

  updateOutput(getValue());

  return true;
}

Value GpioOr::getValue() const {
  Value result = Value::LOW;
  for (auto const &value : m_valueMap) {
    if (Value::HIGH == value.second) {
      result = Value::HIGH;
    }
  }
}

size_t GpioOr::getPinNumber() const { return m_gpioOutput->getPinNumber(); }

void GpioOr::updateOutput(const Value value) {
  m_gpioOutput->setValue(0, value);
}

} // gpio
