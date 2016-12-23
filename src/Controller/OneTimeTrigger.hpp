#pragma once

#include <Gpio/IGpio.hpp>

#include <thread>

// Class
namespace time_trigger {

class OneTimeTrigger {
public:
  OneTimeTrigger(const gpio::IGpioPtr &gpio,
                 const logger::SysLoggerPtr &sysLogger);
  ~OneTimeTrigger();

  void addTrigger(const size_t duration);

private:
  void threadFn(size_t duration);

  gpio::IGpioPtr m_gpio;

  const logger::SysLoggerPtr m_sysLogger;

  std::thread m_thread;
  bool m_stopThread{false};
};

using OneTimeTriggerPtr = std::shared_ptr<OneTimeTrigger>;
}
