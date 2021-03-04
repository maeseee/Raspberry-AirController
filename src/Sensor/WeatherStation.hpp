#pragma once

#include <Sensor/ISensor.hpp>
#include <Threading.hpp>

#include <string>

// FWD
namespace logger
{
class SysLogger;
using SysLoggerPtr = std::shared_ptr<SysLogger>;
} // namespace logger

// Class
namespace sensor
{

class WeatherStation : public sensor::ISensor, public threading::Threading
{
public:
    WeatherStation(const logger::SysLoggerPtr& sysLogger);

    SensorDataCPtr getData() const override;

    void recall() override;

private:
    void updateData();

    SensorDataPtr m_data;

    const logger::SysLoggerPtr m_sysLogger;
    const size_t m_loggerIdTemp{0};
    const size_t m_loggerIdHum{0};
    const size_t m_loggerIdCon{0};
};

static std::string m_receivedData; // will hold the url's contents

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up);
} // namespace sensor
