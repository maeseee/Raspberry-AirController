#ifndef THREADING_H
#define THREADING_H

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
  size_t m_intervall;
  bool m_stopThread{false};
};

} // threading
#endif
