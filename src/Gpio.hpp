#ifndef GPIO_CLASS_H
#define GPIO_CLASS_H

#include <GpioConstants.hpp>
#include <IGpio.hpp>

#include <memory>

namespace gpio {

/**
 * @brief The Gpio class controls one GPIO pin
 */
class Gpio : public IGpio {
public:
  Gpio();
  explicit Gpio(const size_t gnum); // create a GPIO object that controls GPIOx,
                                    // where x is
                                    // passed to this constructor

  bool exportGpio() override;
  bool unexportGpio() override;

  bool setDirection(const Direction dir) override;
  Direction getDirection() const override;

  bool setValue(const Value val) override;
  Value getValue() const override;

private:
  size_t m_gpioNumber; // GPIO number associated with the instance of an object
};

bool isRealBoard();

} // gpio
#endif
