#include "RotiController.hpp"
#include <Constants.hpp>

#include <cassert>
#include <cmath>

namespace roti_controller {

RotiController::RotiController(const sensor::ISensorPtr &indoorSensor,
                               const sensor::ISensorPtr &outdoorSensor,
                               const gpio::IGpioPtr &gpioRoti)
    : threading::Threading(CALL_INTERVALL_ROTI), m_indoorSensor(indoorSensor),
      m_outdoorSensor(outdoorSensor), m_gpioRoti(gpioRoti) {
  assert(m_indoorSensor);
  assert(m_outdoorSensor);
  assert(m_gpioRoti);
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

bool RotiController::shouldBeEnabled(const float indoor,
                                     const float outdoor) const {
  bool increaseIndoor = SET_HUM > indoor;
  bool isOutdoorHigher = indoor < outdoor;

  return increaseIndoor ^ isOutdoorHigher;
}

void RotiController::recall() {
  // grap sensor values
  sensor::SensorData indoor = m_indoorSensor->getData();
  if (std::numeric_limits<float>::min() == indoor.temperature) {
    // Invalid sensor data
    return;
  }
  sensor::SensorData outdoor = m_indoorSensor->getData();
  if (std::numeric_limits<float>::min() == outdoor.temperature) {
    // Invalid sensor data
    return;
  }

  // calculate absHum
  const float absHumIndoor =
      relHumidityToAbs(indoor.temperature, indoor.humidity);
  const float absHumOutdoor =
      relHumidityToAbs(outdoor.temperature, outdoor.humidity);

  // set roti output
  if (shouldBeEnabled(absHumIndoor, absHumOutdoor)) {
    m_gpioRoti->setValue(gpio::Value::HIGH);
  } else {
    m_gpioRoti->setValue(gpio::Value::LOW);
  }
}
}