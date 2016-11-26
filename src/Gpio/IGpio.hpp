#pragma once

#include <Gpio/GpioConstants.hpp>

#include <memory>

// FWD
namespace logger {
class SysLogger;
using SysLoggerPtr = std::shared_ptr<SysLogger>;
}

// Class
namespace gpio {

/**
 * @brief The Gpio class controls one GPIO pin
 */
class IGpio {
public:
  virtual ~IGpio() {}

  /**
   * @brief setdir_gpio Set the direction to the GPIO
   * @param controllerId identifies the controller calling this function
   * @param dir Direction
   * @return true for succesful
   */
  virtual bool setDirection(const size_t controllerId, const Direction dir) = 0;

  /**
  * @brief getDirection returns the direction of the gpio
  */
  virtual Direction getDirection() const = 0;

  /**
   * @brief setval_gpio Set the value to the GPIO
   * @param controllerId identifies the controller calling this function
   * @param val Value
   * @return true if value has been set
   */
  virtual bool setValue(const size_t controllerId, const Value val) = 0;

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

/**
 * @brief isRealBoard Checks wether the current infrastructure is real or not
 * @return true if real
 */
bool isRealBoard();

/**
 * @brief createGpio create a gpio conserning if the board is real or not
 * @param function funtion for the gpio
 * @param dir direction
 * @param val value
 * @param sysLogger system logger
 * @return
 */
IGpioPtr createGpio(const Function function, const Direction dir,
                    const Value val, const logger::SysLoggerPtr &sysLogger);
}
