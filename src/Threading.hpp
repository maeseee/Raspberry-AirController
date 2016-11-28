#pragma once

#include <thread>

namespace threading {

class Threading {
public:
  Threading(const size_t intervall);
  virtual ~Threading();

  virtual void stopThreading() final;
  virtual void waitStopped() final;

  virtual void recall() = 0;

private:
  void threadFn();

  std::thread m_thread;
  const size_t m_intervall;
  size_t m_timeCounter {0};
  bool m_stopThread{false};
};
}
