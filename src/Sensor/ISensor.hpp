#ifndef ISENSOR_H
#define ISENSOR_H

#include <memory>

namespace sensor {

struct SensorData {
  float temperature; // [°C] temperature
  float humidity;    // [%] humadity
};

class ISensor {
public:
  virtual ~ISensor() {}

  virtual SensorData getData() const = 0;
};
using ISensorPtr = std::shared_ptr<ISensor>;

} // sensor
#endif
