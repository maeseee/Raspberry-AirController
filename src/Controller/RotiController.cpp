#include "RotiController.hpp"
#include <Constants.hpp>
#include <SysLogger.hpp>

#include <cassert>
#include <sstream>

namespace controller {

static const size_t LOG_INTERVALL = 30 * MIN_TO_SEC / CALL_INTERVALL_ROTI;

RotiController::RotiController(const sensor::ISensorPtr &indoorSensor,
                               const sensor::ISensorPtr &outdoorSensor,
                               const gpio::IGpioPtr &gpioRoti,
                               const logger::SysLoggerPtr &sysLogger)
    : threading::Threading(CALL_INTERVALL_ROTI), m_indoorSensor(indoorSensor),
      m_outdoorSensor(outdoorSensor), m_gpioRoti(gpioRoti),
      m_sysLogger(sysLogger) {
  assert(m_indoorSensor);
  assert(m_outdoorSensor);
  assert(m_gpioRoti);

  m_loggerId = m_sysLogger->getId("RotiController");
}

RotiController::~RotiController() {
  m_gpioRoti->setValue(m_loggerId, gpio::Value::LOW);
}

bool RotiController::shouldBeEnabled(const float indoor, const float outdoor,
                                     const float set) const {
  bool increaseIndoor = set > indoor;
  bool isOutdoorHigher = indoor < outdoor;

  // decreaseIndoor  OutdoorLower   -> off
  // decreaseIndoor  OutdoorHigher  -> on
  // increaseIndoor  OutdoorLower  -> on
  // increaseIndoor  OutdoorHigher  -> off
  return increaseIndoor ^ isOutdoorHigher;
}

void RotiController::recall() {
  // grap sensor values
  sensor::SensorData indoor = m_indoorSensor->getData();
  if (std::numeric_limits<float>::min() == indoor.temperature) {
    // Invalid sensor data
    return;
  }
  sensor::SensorData outdoor = m_outdoorSensor->getData();
  if (std::numeric_limits<float>::min() == outdoor.temperature) {
    // Invalid sensor data
    return;
  }

  // calculate absHum
  const float absHumIndoor =
      relHumidityToAbs(indoor.temperature, indoor.humidity);
  const float absHumOutdoor =
      relHumidityToAbs(outdoor.temperature, outdoor.humidity);
  const float absHumSet = relHumidityToAbs(SET_TEMP, SET_HUM);

  static size_t counter = 0;
  if (0 == counter) {
    std::stringstream logSs;
    logSs << "AbsHumIndoor: " << absHumIndoor
          << "\tAbsHumOutdoor: " << absHumOutdoor
          << "\tAbsHumSet: " << absHumSet;
    m_sysLogger->logMsg(m_loggerId, logSs.str());
    ++counter;
    counter %= LOG_INTERVALL;
  }

  // set roti output
  if (shouldBeEnabled(absHumIndoor, absHumOutdoor, absHumSet)) {
    m_gpioRoti->setValue(m_loggerId, gpio::Value::HIGH);
  } else {
    m_gpioRoti->setValue(m_loggerId, gpio::Value::LOW);
  }
}
}
