#ifndef GPIO_COLLECTOR_CLASS_H
#define GPIO_COLLECTOR_CLASS_H

#include <IGpio.hpp>

#include <memory>
#include <vector>
#include <functional>

namespace gpio {

/**
 * @brief The Gpio class controls one GPIO pin
 */
class GpioCollector : public IGpio {
public:
  explicit GpioCollector(const IGpioPtr& gpio);

  bool setDirection(const Direction dir) override;
  Direction getDirection() const override;

  bool setValue(const Value val) override;
  Value getValue() const override;

  size_t getPinNumber() const override;

  void addValueFn(const std::function<Value()>& fn);

  Value updateValue();

private:
  const IGpioPtr m_gpio;
  std::vector<std::function<Value()>> m_valueVector;
};

} // gpio
#endif
