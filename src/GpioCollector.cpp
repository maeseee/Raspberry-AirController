#include "GpioCollector.hpp"

#include <cassert>

namespace gpio {

GpioCollector::GpioCollector(const IGpioPtr &gpio) : m_gpio(gpio) {}

bool GpioCollector::setDirection(const Direction dir) {
  // Direction can only be out for the GpioCollection
  assert(Direction::OUT == dir);
  return m_gpio->setDirection(dir);
}

Direction GpioCollector::getDirection() const { return m_gpio->getDirection(); }

bool GpioCollector::setValue(const Value /*val*/) {
  // just ignore the value as it must be safed by the caller
  updateValue();
}

Value GpioCollector::getValue() const { return m_gpio->getValue(); }

size_t GpioCollector::getPinNumber() const { return m_gpio->getPinNumber(); }

void GpioCollector::addValueFn(const std::function<Value()> &fn) {
  m_valueVector.push_back(fn);
}

Value GpioCollector::updateValue() {
  Value result = Value::LOW;
  if (m_valueVector.size() != 0) {
    for (const std::function<Value()> &fn : m_valueVector) {
      Value fnReturn = fn();
      if (Value::HIGH == fnReturn) {
        result = fnReturn;
      }
    }
    return result;
  }
}

} // gpio
