#include "Humidity.hpp"

#include <cassert>
#include <cmath>
#include <unistd.h> // for sleep

namespace humidity {

static const size_t CALL_INTERVALL = 5 * 60; // call upcate intervall for thread
static constexpr float SET_HUM = 50.0;       // set point for humidity [%]
static constexpr float SET_TEMP = 22.0;      // set point for temerautre [°C]

HumidityController::HumidityController(const sensor::ISensorPtr &indoorSensor,
                                       const sensor::ISensorPtr &outdoorSensor,
                                       const gpio::IGpioPtr &gpio)
    : m_rotiEnabler(enabler::Enabler(relHumidityToAbs(SET_TEMP, SET_HUM))),
      m_indoorSensor(indoorSensor), m_outdoorSensor(outdoorSensor),
      m_gpio(gpio) {
  assert(m_indoorSensor);
  assert(m_outdoorSensor);
  assert(m_gpio);

  m_thread = std::thread(&HumidityController::threadFn, this);
}

HumidityController::~HumidityController() {
  m_stopThread = true;
  m_thread.join();
}

void HumidityController::setEnabled(bool enable) { m_isEnabled = enable; }

float HumidityController::relHumidityToAbs(const float tempC,
                                           const float humidityRel) {
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

float HumidityController::absHumidityToRel(const float tempC,
                                           const float humidityAbs) {
  float absHumidity100 = relHumidityToAbs(tempC, 100);
  return humidityAbs / absHumidity100 * 100;
}

void HumidityController::threadFn() {
  while (!m_stopThread) {
    static int timeCounter = 0;
    if (0 == timeCounter) {
      recall();
    }
    ++timeCounter;
    timeCounter %= CALL_INTERVALL;
    sleep(1);
  }
}

void HumidityController::recall() {
  sensor::SensorData indoor = m_indoorSensor->getData();
  if (std::numeric_limits<float>::min() == indoor.temperature) {
    // Invalid sensor data
    return;
  }
  const float absHumIndoor =
      relHumidityToAbs(indoor.temperature, indoor.humidity);

  sensor::SensorData outdoor = m_indoorSensor->getData();
  if (std::numeric_limits<float>::min() == outdoor.temperature) {
    // Invalid sensor data
    return;
  }
  const float absHumOutdoor =
      relHumidityToAbs(outdoor.temperature, outdoor.humidity);

  if (m_rotiEnabler.shouldBeEnabled(absHumIndoor, absHumOutdoor)) {
    m_gpio->setValue(gpio::Value::ON);
  } else {
    m_gpio->setValue(gpio::Value::OFF);
  }
}
}
