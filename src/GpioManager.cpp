#include "GpioManager.hpp"
#include <Gpio.hpp>

#include <cassert>

namespace gpio {

static constexpr size_t GPIO_MAIN = 3;
static constexpr size_t GPIO_ROTI = 5;
static constexpr size_t GPIO_RES = 7;
static constexpr size_t GPIO_TIME = 8;

GpioManager::GpioManager() {
  m_outputs[Function::Main] = std::make_shared<Gpio>(GPIO_MAIN);
  m_outputs[Function::Roti] = std::make_shared<Gpio>(GPIO_ROTI);
  m_outputs[Function::Reserve] = std::make_shared<Gpio>(GPIO_RES);
  m_outputs[Function::Time] = std::make_shared<Gpio>(GPIO_TIME);

  initGpios();
}

Value GpioManager::getValue(const Function function) const {
  assert(function < Function::Size);

  return m_outputs[function]->getValue();
}

bool GpioManager::setValue(const Function function, const Value value) {
  assert(function < Function::Size);

  // only setValue if it is not already set
  if (value == getValue(function)) {
    return true;
  } else {
    return m_outputs[function]->setValue(value);
  }
}

bool GpioManager::initGpios() {
  bool result = true;

  for (const GpioPtr &output : m_outputs) {
    // export gpio's
    result = result && output->exportGpio();

    // set direction
    result = result && output->setDirection(Direction::OUT);

    // set output off
    result = result && output->setValue(Value::OFF);
  }

  return result;
}

} // gpio
