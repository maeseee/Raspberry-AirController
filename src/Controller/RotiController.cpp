#include "RotiController.hpp"
#include <Constants.hpp>
#include <Controller/ControllerIdGenerator.hpp>
#include <SysLogger.hpp>

#include <cassert>
#include <cmath>
#include <sstream>

namespace controller {

static const size_t LOG_INTERVALL = 30 * MIN_TO_SEC / CALL_INTERVALL_ROTI;

RotiController::RotiController(const sensor::ISensorPtr &indoorSensor,
                               const sensor::ISensorPtr &outdoorSensor,
                               const gpio::IGpioPtr &gpioRoti)
    : threading::Threading(CALL_INTERVALL_ROTI), m_indoorSensor(indoorSensor),
      m_outdoorSensor(outdoorSensor), m_gpioRoti(gpioRoti) {
  assert(m_indoorSensor);
  assert(m_outdoorSensor);
  assert(m_gpioRoti);

  m_controllerId = controller::IdGenerator::generateId();
}

RotiController::~RotiController() {
  m_gpioRoti->setValue(m_controllerId, gpio::Value::LOW);
}

float RotiController::relHumidityToAbs(const float tempC,
                                       const float humidityRel) const {
  // function calculated for
  // Temperatur [°C]  water for 100 % [g/m3]
  // -20              0.9
  // -15	          1.4
  // -10	          2.1
  // -5	              3.3
  // 0	              4.8
  // 5	              6.8
  // 10	              9.4
  // 15	              12.8
  // 20	              17.3
  // 25	              23
  // 30	              30.3
  // 35	              39.6
  // 40	              51.1

  return 4.2431796244 * exp(0.0666427637 * tempC) * humidityRel / 100;
}

float RotiController::absHumidityToRel(const float tempC,
                                       const float humidityAbs) const {
  float absHumidity100 = relHumidityToAbs(tempC, 100);
  return humidityAbs / absHumidity100 * 100;
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
    logSs << "RotiController: AbsHumIndoor: " << absHumIndoor
          << "\tAbsHumOutdoor: " << absHumOutdoor
          << "\tAbsHumSet: " << absHumSet;
    logger::SysLogger::instance().log(logSs.str());
    ++counter;
    counter %= LOG_INTERVALL;
  }

  // set roti output
  if (shouldBeEnabled(absHumIndoor, absHumOutdoor, absHumSet)) {
    m_gpioRoti->setValue(m_controllerId, gpio::Value::HIGH);
  } else {
    m_gpioRoti->setValue(m_controllerId, gpio::Value::LOW);
  }
}
}
