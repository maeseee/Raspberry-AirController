#ifndef TIME_TRIGGER_H
#define TIME_TRIGGER_H

#include <GpioConstants.hpp>

#include <ctime>
#include <thread>

namespace time_trigger {

struct OnPeriod {
    size_t onTime; // [s]
    size_t offTime; // [s]
};

class TimeTrigger {
public:
    /**
   * @brief TimeTrigger calls the std::function in the specific intervall
   * @param on [s] from 00:00
   * @param off [s] from 00:00
   */
  TimeTrigger(const size_t on, const size_t off, gpio::GpioSetter function);
  ~TimeTrigger();

  void threadFn();

private:
  std::thread m_thread;
  bool m_stopThread{false};

  OnPeriod m_period;
  gpio::GpioSetter fn_gpioSetter;
};

static std::string receivedData; // will hold the url's contents

static size_t writeCallback(char *buf, size_t size, size_t nmemb, void *up);

} //time_trigger

#endif //TIME_TRIGGER_H
