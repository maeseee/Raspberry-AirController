#include "TempController.hpp"
#include <Constants.hpp>

#include <cassert>
#include <cmath>
#include <ctime>
#include <iostream>
#include <unistd.h> // for sleep

namespace temp_controller {

static const size_t SUMMER_ON = 2 * 60 * 60;
static const size_t WINTER_ON = 14 * 60 * 60;
static const size_t ON_DURATION = 5 * 60;

TempController::TempController(
    const gpio::IGpioPtr &gpioMainSystem)
    : m_gpio(gpioMainSystem) {
  assert(m_gpio);

  m_thread = std::thread(&TempController::threadFn, this);
}

TempController::~TempController() {
  m_stopThread = true;
  m_thread.join();
}

bool TempController::shouldWarm() const {
  time_t t = time(0); // get time now
  struct tm *now = localtime(&t);

  std::cout << "Date of today is: " << (now->tm_year + 1900) << '-'
            << (now->tm_mon + 1) << '-' << now->tm_mday << std::endl;

  size_t month = now->tm_mon + 1;
  if (month < 7 && month > 8) {
    // Cool in the months July and August
    return false;
  } else {
    return true;
  }
}

void TempController::threadFn() {
  while (!m_stopThread) {
    static int timeCounter = 0;
    if (0 == timeCounter) {
      recall();
    }
    ++timeCounter;
    timeCounter %= CALL_INTERVALL_TEMP;
    sleep(1);
  }
}

void TempController::recall() {
  const bool isShouldWarm = shouldWarm();
  if ((nullptr == m_timer) || (m_oldShouldWarmup != isShouldWarm)) {
    m_timer = nullptr; // Destruct current timer
    if (isShouldWarm) {
      m_timer = std::make_shared<time_trigger::TimeTrigger>(
          WINTER_ON, WINTER_ON + ON_DURATION, m_gpio);
    } else {
      m_timer = std::make_shared<time_trigger::TimeTrigger>(
          SUMMER_ON, SUMMER_ON + ON_DURATION, m_gpio);
    }
    m_oldShouldWarmup = isShouldWarm;
  }
}
}
