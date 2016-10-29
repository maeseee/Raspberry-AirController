#pragma once

#include <Gpio/GpioConstants.hpp>

#include <memory>

namespace gpio {

/**
 * @brief The Gpio class controls one GPIO pin
 */
class IGpio {
public:
  virtual ~IGpio() {}

  /**
   * @brief setdir_gpio Set the direction to the GPIO
   * @param dir Direction
   * @return true for succesful
   */
  virtual bool setDirection(const Direction dir) = 0;

  /**
  * @brief getDirection returns the direction of the gpio
  */
  virtual Direction getDirection() const = 0;

  /**
   * @brief setval_gpio Set the value to the GPIO
   * @param val Value
   * @return true for succesful
   */
  virtual bool setValue(const Value val) = 0;

  /**
   * @brief getval_gpio Get the value to the GPIO
   * @return Value (Value::INVALID for error)
   */
  virtual Value getValue() const = 0;

  /**
   * @brief getFunction returns the function of the pin
   */
  virtual size_t getPinNumber() const = 0;
};
using IGpioPtr = std::shared_ptr<IGpio>;
}
