#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <IGpio.hpp>
#include <TimeTrigger.hpp>

#include <thread>

namespace temp_controller {

class TempController {
public:
  /**
 * @brief TemperatureController turns on and off the air system
 * @param gpioMainSystem gpio for switching on and off
 */
  TempController(const gpio::IGpioPtr &gpioMainSystem);
  ~TempController();

private:
  bool shouldWarm() const;

  void threadFn();
  void recall();

  gpio::IGpioPtr m_gpio;
  time_trigger::TimeTriggerPtr m_timer;
  bool m_oldShouldWarmup{true};

  std::thread m_thread;
  bool m_stopThread{false};
};

} // humidity
#endif
