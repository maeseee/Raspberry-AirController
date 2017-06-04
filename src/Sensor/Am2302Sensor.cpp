#include "Am2302Sensor.hpp"
#include <Sensor/pi_2_dht_read.hpp>
#include <SysLogger.hpp>
#include <Utility/Constants.hpp>

#include <cassert>

namespace sensor
{

Am2302Sensor::Am2302Sensor(const gpio::IGpioPtr& sensor, const logger::SysLoggerPtr& sysLogger)
    : threading::Threading(CALL_INTERVALL_AM2302)
    , m_sensor(sensor)
    , m_sysLogger(sysLogger)
    , m_loggerIdTemp(sysLogger->generateId("Indoor Temperature"))
    , m_loggerIdHum(sysLogger->generateId("Indoor Humidity"))

{
    assert(m_sensor);


    setInitialized();
}

SensorDataPtr Am2302Sensor::getData() const
{
    if (INVALID_FLOAT >= m_temperature) {
        m_sysLogger->logError(m_loggerIdTemp, "Invalid indoor value");
        return nullptr;
    } else if (INVALID_FLOAT >= m_humidity) {
        m_sysLogger->logError(m_loggerIdHum, "Invalid indoor value");
        return nullptr;
    } else {
        return std::make_shared<SensorData>(m_temperature, m_humidity);
    }
}

void Am2302Sensor::recall()
{
    float humidity = 0;
    float temperature = 0;

    const DhtState dhtState = pi_2_dht_read(DHT22, m_sensor->getPinNumber(), &humidity, &temperature);

    if (DhtState::SUCCESS == dhtState) {
        m_humidity = humidity;
        m_temperature = temperature;
        m_sysLogger->logSensorValue(m_loggerIdTemp, m_temperature);
        m_sysLogger->logSensorValue(m_loggerIdHum, m_humidity);
    } else {
        m_humidity = INVALID_FLOAT;
        m_temperature = INVALID_FLOAT;
        m_sysLogger->logError(m_loggerIdTemp,
                              "1wire bus return invalid value of " + std::to_string(static_cast<size_t>(dhtState)));
    }
}
}
