#pragma once

#include <limits>
#include <memory>

namespace sensor
{
static constexpr double INVALID_double = std::numeric_limits<double>::min();

class SensorData
{
public:
    explicit SensorData(const size_t temperature, const size_t humidity)
        : m_temperature(temperature)
        , m_humidity(humidity)
    {
    }

    double m_temperature; // [°C] temperature
    double m_humidity;    // [%] humadity
};
using SensorDataPtr = std::shared_ptr<SensorData>;
using SensorDataCPtr = std::shared_ptr<const SensorData>;

class ISensor
{
public:
    virtual ~ISensor() = default;

    virtual SensorDataCPtr getData() const = 0;
};
using ISensorPtr = std::shared_ptr<ISensor>;
} // namespace sensor
