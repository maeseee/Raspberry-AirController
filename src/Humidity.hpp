#ifndef HUMIDITY_H
#define HUMIDITY_H

#include <Enabler.hpp>
#include <IGpio.hpp>
#include <ISensor.hpp>

#include <thread>

namespace gpio {
class GpioCollector;
using GpioCollectorPtr = std::shared_ptr<GpioCollector>;
}
namespace humidity {

class HumidityController {
public:
  /**
 * @brief HumidityController
 * @param setPointHumidity [%]
 */
  HumidityController(const sensor::ISensorPtr &indoorSensor,
                     const sensor::ISensorPtr &outdoorSensor,
                     const gpio::IGpioPtr &gpioRoti,
                     const gpio::GpioCollectorPtr &gpioMainSystem);
  ~HumidityController();

private:
  float relHumidityToAbs(const float tempC, const float humidityRel) const;
  float absHumidityToRel(const float tempC, const float humidityAbs) const;

  void threadFn();
  void recall();

  enabler::Enabler m_rotiEnabler;

  sensor::ISensorPtr m_indoorSensor;
  sensor::ISensorPtr m_outdoorSensor;
  gpio::IGpioPtr m_gpioRoti;
  gpio::GpioCollectorPtr m_gpioMainSystem;

  float m_measuredTempIndoor;  // [°C]
  float m_measuredTempOutdoor; // [°C]

  float m_measuredHumIndoor;  // [%]
  float m_measuredHumOutdoor; // [%]

  std::thread m_thread;
  bool m_stopThread{false};
};

} // humidity
#endif
