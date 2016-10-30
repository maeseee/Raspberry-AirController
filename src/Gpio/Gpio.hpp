#pragma once

#include <Gpio/GpioConstants.hpp>
#include <Gpio/IGpio.hpp>

namespace gpio {

/**
 * @brief The Gpio class controls one GPIO pin
 */
class Gpio : public IGpio {
public:
  explicit Gpio(const Function gnum, const Direction dir, const Value val);

  ~Gpio();

  bool setDirection(const size_t controllerId, const Direction dir) override;
  Direction getDirection() const override;

  bool setValue(const size_t controllerId, const Value val) override;
  Value getValue() const override;

  size_t getPinNumber() const override;

private:
  /**
   * @brief export_gpio Export the GPIO
   * @return true for successful
   */
  bool exportGpio();

  /**
   * @brief unexport_gpio Unexport the GPIO
   * @return true for succesful
   */
  bool unexportGpio();

  size_t m_gpioNumber; // GPIO number associated with the instance of an object
};

bool isRealBoard();
}
