#pragma once

#include <memory>

namespace sensor
{

struct SensorData {
    float temperature; // [°C] temperature
    float humidity;    // [%] humadity
};

class ISensor
{
public:
    virtual ~ISensor() = default;

    virtual SensorData getData() const = 0;
};
using ISensorPtr = std::shared_ptr<ISensor>;
}
