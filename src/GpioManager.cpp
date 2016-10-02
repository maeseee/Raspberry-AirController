#include "GpioManager.hpp"
#include <Gpio.hpp>
#include <GpioSim.hpp>

#include <cassert>
#include <iostream>

namespace gpio {

static constexpr size_t GPIO_MAIN = 3;
static constexpr size_t GPIO_ROTI = 5;
static constexpr size_t GPIO_RES = 7;
static constexpr size_t GPIO_TIME = 8;

GpioManager::GpioManager() {
  if (isRealBoard()) {
    m_outputs[Function::Main] = std::make_shared<Gpio>(GPIO_MAIN);
    m_outputs[Function::Roti] = std::make_shared<Gpio>(GPIO_ROTI);
    m_outputs[Function::Reserve] = std::make_shared<Gpio>(GPIO_RES);
    m_outputs[Function::Time] = std::make_shared<Gpio>(GPIO_TIME);
  } else {
    m_outputs[Function::Main] = std::make_shared<GpioSim>("MainSystem");
    m_outputs[Function::Roti] = std::make_shared<GpioSim>("Roti");
    m_outputs[Function::Reserve] = std::make_shared<GpioSim>("Reserve");
    m_outputs[Function::Time] = std::make_shared<GpioSim>("Timer");
  }

  initGpios();
}

IGpioPtr GpioManager::getGpio(Function function) const {
  assert(function < Function::Size);

  return m_outputs[function];
}

bool GpioManager::initGpios() {
  bool result = true;

  for (const IGpioPtr &output : m_outputs) {
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
