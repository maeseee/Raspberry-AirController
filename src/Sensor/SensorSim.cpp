#include "SensorSim.hpp"

namespace sensor {

SensorData SensorSim::getData() const { return SensorData{22.0, 50}; }

} // sensor
