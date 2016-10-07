#include "GpioManager.hpp"
#include <Gpio.hpp>
#include <GpioSim.hpp>

#include <cassert>
#include <iostream>

namespace gpio {

// outputs
static constexpr size_t GPIO_MAIN = 3;
static constexpr size_t GPIO_ROTI = 5;
static constexpr size_t GPIO_RES = 7;
static constexpr size_t GPIO_TIME = 8;

// inputs
static constexpr size_t GPIO_AM2302 = 9;

GpioManager::GpioManager() {
  if (isRealBoard()) {
    m_gpios[Function::Main] = std::make_shared<Gpio>(GPIO_MAIN);
    m_gpios[Function::Roti] = std::make_shared<Gpio>(GPIO_ROTI);
    m_gpios[Function::Time] = std::make_shared<Gpio>(GPIO_TIME);
    m_gpios[Function::Reserve] = std::make_shared<Gpio>(GPIO_RES);
    m_gpios[Function::Am2302] = std::make_shared<Gpio>(GPIO_AM2302);
  } else {
    m_gpios[Function::Main] = std::make_shared<GpioSim>("MainSystem");
    m_gpios[Function::Roti] = std::make_shared<GpioSim>("Roti");
    m_gpios[Function::Reserve] = std::make_shared<GpioSim>("Reserve");
    m_gpios[Function::Time] = std::make_shared<GpioSim>("Timer");
    m_gpios[Function::Am2302] = std::make_shared<GpioSim>("Am2302");
  }

  initGpios();
}

IGpioPtr GpioManager::getGpio(Function function) const {
  assert(function < Function::Size);

  return m_gpios[function];
}

bool GpioManager::initGpios() {
  bool result = true;

  for (const IGpioPtr &output : m_gpios) {
    // export gpio's
    result = result && output->exportGpio();

    // set direction
    // careful with the output (input would be more safe)
    result = result && output->setDirection(Direction::OUT);

    // set output off
    result = result && output->setValue(Value::LOW);
  }

  return result;
}

} // gpio
