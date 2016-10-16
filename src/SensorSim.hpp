#ifndef SENSOR_SIM_H
#define SENSOR_SIM_H

#include <ISensor.hpp>

namespace sensor {

/**
 * @brief The Gpio class controls one GPIO pin
 */
class SensorSim : public ISensor {
public:
  SensorData getData() const override;
};
} // sensor
#endif
