#ifndef GPIO_MANAGER_H
#define GPIO_MANAGER_H

#include <GpioConstants.hpp>

#include <array>
#include <memory>

namespace gpio {

// FWD
class IGpio;
using IGpioPtr = std::shared_ptr<IGpio>;

// Implementation
class GpioManager {
public:
  GpioManager();

  IGpioPtr getGpio(Function function) const;

private:
  bool initGpios();

  std::array<IGpioPtr, Function::Size> m_gpios;
};

} // gpio
#endif
