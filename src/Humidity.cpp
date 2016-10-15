#include "Humidity.hpp"
#include <GpioCollector.hpp>

#include <cassert>
#include <cmath>
#include <unistd.h> // for sleep

namespace humidity {

static const size_t CALL_INTERVALL = 5 * 60; // call upcate intervall for thread
static constexpr float SET_HUM = 50.0;       // set point for humidity [%]
static constexpr float SET_TEMP = 22.0;      // set point for temerautre [°C]

HumidityController::HumidityController(
    const sensor::ISensorPtr &indoorSensor,
    const sensor::ISensorPtr &outdoorSensor, const gpio::IGpioPtr &gpioRoti,
    const gpio::GpioCollectorPtr &gpioMainSystem)
    : m_rotiEnabler(enabler::Enabler(relHumidityToAbs(SET_TEMP, SET_HUM))),
      m_indoorSensor(indoorSensor), m_outdoorSensor(outdoorSensor),
      m_gpioRoti(gpioRoti), m_gpioMainSystem(gpioMainSystem) {
  assert(m_indoorSensor);
  assert(m_outdoorSensor);
  assert(m_gpioRoti);

  m_thread = std::thread(&HumidityController::threadFn, this);
}

HumidityController::~HumidityController() {
  m_stopThread = true;
  m_thread.join();
}

float HumidityController::relHumidityToAbs(const float tempC,
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

float HumidityController::absHumidityToRel(const float tempC,
                                           const float humidityAbs) const {
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
  if (gpio::Value::LOW == m_gpioMainSystem->getValue()) {
    // with switched of main system the roti makes no sense
    return;
  }

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
    m_gpioRoti->setValue(gpio::Value::HIGH);
  } else {
    m_gpioRoti->setValue(gpio::Value::LOW);
  }
}
}
