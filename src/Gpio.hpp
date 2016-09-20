#ifndef GPIO_CLASS_H
#define GPIO_CLASS_H

#include <GpioConstants.hpp>

#include <memory>

namespace gpio {

/**
 * @brief The Gpio class controls one GPIO pin
 */
class Gpio {
public:
  Gpio();

  explicit Gpio(const size_t gnum); // create a GPIO object that controls GPIOx,
                                    // where x is
                                    // passed to this constructor

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

  /**
   * @brief setdir_gpio Set the direction to the GPIO
   * @param dir Direction
   * @return true for succesful
   */
  bool setDirection(const Direction dir);

  /**
   * @brief setval_gpio Set the value to the GPIO
   * @param val Value
   * @return true for succesful
   */
  bool setValue(const Value val);

  /**
   * @brief getval_gpio Get the value to the GPIO
   * @return Value (Value::INVALID for error)
   */
  Value getValue() const;

private:
  size_t m_gpioNumber; // GPIO number associated with the instance of an object
  Direction m_dir{Direction::UNSET};
};
using GpioPtr = std::shared_ptr<Gpio>;

} // gpio
#endif
