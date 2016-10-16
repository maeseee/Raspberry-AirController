#ifndef HUMIDITY_H
#define HUMIDITY_H

#include <IGpio.hpp>
#include <ISensor.hpp>

#include <thread>

namespace gpio {
class GpioCollector;
using GpioCollectorPtr = std::shared_ptr<GpioCollector>;
}
namespace roti_controller {

class RotiController {
public:
  /**
 * @brief RotiController controlls the humidity in the room
 * @param indoorSensor sensor for indoor humidity [%]
 * @param outdoorSensor sensor for outdoor humidity [%]
 * @param gpioRoti gpio for controlling the roti
 */
  RotiController(const sensor::ISensorPtr &indoorSensor,
                 const sensor::ISensorPtr &outdoorSensor,
                 const gpio::IGpioPtr &gpioRoti);
  ~RotiController();

private:
  float relHumidityToAbs(const float tempC, const float humidityRel) const;
  float absHumidityToRel(const float tempC, const float humidityAbs) const;

  bool shouldBeEnabled(const float indoor, const float outdoor) const;

  void threadFn();
  void recall();

  sensor::ISensorPtr m_indoorSensor;
  sensor::ISensorPtr m_outdoorSensor;
  gpio::IGpioPtr m_gpioRoti;

  float m_measuredTempIndoor;  // [°C]
  float m_measuredTempOutdoor; // [°C]

  float m_measuredHumIndoor;  // [%]
  float m_measuredHumOutdoor; // [%]

  std::thread m_thread;
  bool m_stopThread{false};
};

} // humidity
#endif
