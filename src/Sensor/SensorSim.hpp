#pragma once

#include <Sensor/ISensor.hpp>

namespace sensor {

/**
 * @brief The Gpio class controls one GPIO pin
 */
class SensorSim : public ISensor {
public:
  SensorData getData() const override;
};
}
