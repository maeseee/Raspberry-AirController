#include "HumLimitController.hpp"
#include <Constants.hpp>
#include <SysLogger.hpp>

#include <cassert>
#include <cmath>
#include <ctime>

namespace controller {

HumLimitController::HumLimitController(const sensor::ISensorPtr &indoorSensor,
                                       const sensor::ISensorPtr &outdoorSensor,
                                       const gpio::IGpioPtr &gpioMainSystem,
                                       const logger::SysLoggerPtr &sysLogger)
    : threading::Threading(CALL_INTERVALL_HUMLIMIT),
      m_indoorSensor(indoorSensor), m_outdoorSensor(outdoorSensor),
      m_gpio(gpioMainSystem), m_sysLogger(sysLogger) {
  assert(m_indoorSensor);
  assert(m_outdoorSensor);
  assert(m_gpio);

  m_loggerId = m_sysLogger->getId("HumLimitController");
}

void HumLimitController::recall() {

  const sensor::SensorData indoorData = m_indoorSensor->getData();
  const sensor::SensorData outdoorData = m_outdoorSensor->getData();

  // calculate absHum
  const float absHumIndoor =
      relHumidityToAbs(indoorData.temperature, indoorData.humidity);
  const float absHumOutdoor =
      relHumidityToAbs(outdoorData.temperature, outdoorData.humidity);
  const float absHumSetPoint = relHumidityToAbs(SET_TEMP, SET_HUM);
  const float absHumUpperLimit =
      relHumidityToAbs(SET_TEMP, SET_HUM + HUM_LIMIT_TOLERANCE);
  const float absHumLowerLimit =
      relHumidityToAbs(SET_TEMP, SET_HUM - HUM_LIMIT_TOLERANCE);

  if ((absHumUpperLimit < absHumIndoor) && (absHumOutdoor < absHumSetPoint)) {
    // Fresh air can be used for lower the indoor humidity
    m_gpio->setValue(m_loggerId, gpio::Value::HIGH);
  } else if ((absHumLowerLimit > absHumIndoor) &&
             (absHumOutdoor > absHumSetPoint)) {
    // Fresh air can be used for higher the indoor humidity
    m_gpio->setValue(m_loggerId, gpio::Value::HIGH);
  } else {
    // No additional fresh air to controll the indoor humidity
    m_gpio->setValue(m_loggerId, gpio::Value::LOW);
  }
}
}