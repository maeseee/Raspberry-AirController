#pragma once

#include <Sensor/ISensor.hpp>
#include <Threading.hpp>

#include <string>

// FWD
namespace logger
{
class SysLogger;
using SysLoggerPtr = std::shared_ptr<SysLogger>;
}

// Class
namespace sensor
{

class WeatherStation : public sensor::ISensor, public threading::Threading
{
public:
    WeatherStation(const logger::SysLoggerPtr& sysLogger);

    SensorData getData() const override;

    void recall() override;

private:
    void updateData();

    float m_temperature; // [°C]
    float m_humidity;    // [%]

    size_t m_loggerIdTemp{0};
    size_t m_loggerIdHum{0};
    size_t m_loggerIdCon{0};
    const logger::SysLoggerPtr m_sysLogger;
};

static std::string m_receivedData; // will hold the url's contents

static size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up);
}
