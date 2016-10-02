#ifndef GPIO_SIM_H
#define GPIO_SIM_H

#include <IGpio.hpp>

#include <string>

namespace gpio {

/**
 * @brief The Gpio class controls one GPIO pin
 */
class GpioSim : public IGpio {
public:
  explicit GpioSim(const std::string &name);

  bool exportGpio() override;
  bool unexportGpio() override;

  bool setDirection(const Direction dir) override;
  Direction getDirection() const override;

  bool setValue(const Value val) override;
  Value getValue() const override;

private:
  Direction m_dir{Direction::UNSET};
  Value m_val{Value::INVALID};

  std::string m_name;
};
} // gpio
#endif
