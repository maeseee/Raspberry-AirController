#pragma once

#include <Controller/TimeTrigger.hpp>
#include <Gpio/IGpio.hpp>
#include <Sensor/ISensor.hpp>
#include <Threading.hpp>

namespace controller {

class HumLimitController : public threading::Threading {
public:
  /**
 * @brief TemperatureController turns on and off the air system
 * @param gpioMainSystem gpio for switching on and off
 */
  HumLimitController(const sensor::ISensorPtr &indoorSensor,
                     const sensor::ISensorPtr &outdoorSensor,
                     const gpio::IGpioPtr &gpioMainSystem);

  void recall() override;

private:
  bool shouldWarm() const;

  sensor::ISensorPtr m_indoorSensor;
  sensor::ISensorPtr m_outdoorSensor;
  gpio::IGpioPtr m_gpio;

  time_trigger::TimeTriggerPtr m_timer;

  size_t m_controllerId;
};
}
