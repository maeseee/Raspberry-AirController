#pragma once

#include <Gpio/IGpio.hpp>

#include <map>
#include <memory>

namespace gpio {

class GpioOr : public IGpio {
public:
  explicit GpioOr(const IGpioPtr &gpioOutput);

  bool setDirection(const size_t controllerId, const Direction dir) override;
  Direction getDirection() const override;

  bool setValue(const size_t controllerId, const Value val) override;
  Value getValue() const override;

  size_t getPinNumber() const override;

private:
  void updateOutput(const Value value);
  const IGpioPtr m_gpioOutput;

  std::map<size_t, Value> m_valueMap;
};
}
