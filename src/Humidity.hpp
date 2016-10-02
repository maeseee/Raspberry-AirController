#ifndef HUMIDITY_H
#define HUMIDITY_H

#include <Enabler.hpp>
#include <IGpio.hpp>
#include <ISensor.hpp>

#include <thread>

namespace humidity {

class HumidityController {
public:
  /**
 * @brief HumidityController
 * @param setPointHumidity [%]
 */
  HumidityController(const sensor::ISensorPtr &indoorSensor,
                     const sensor::ISensorPtr &outdoorSensor,
                     const gpio::IGpioPtr &gpio);
  ~HumidityController();

  void setEnabled(bool enable);

private:
  static float relHumidityToAbs(const float tempC, const float humidityRel);
  static float absHumidityToRel(const float tempC, const float humidityAbs);

  void threadFn();
  void recall();

  bool m_isEnabled{false};

  enabler::Enabler m_rotiEnabler;

  sensor::ISensorPtr m_indoorSensor;
  sensor::ISensorPtr m_outdoorSensor;
  gpio::IGpioPtr m_gpio;

  float m_measuredTempIndoor;  // [°C]
  float m_measuredTempOutdoor; // [°C]

  float m_measuredHumIndoor;  // [%]
  float m_measuredHumOutdoor; // [%]

  std::thread m_thread;
  bool m_stopThread{false};
};

} // humidity
#endif
