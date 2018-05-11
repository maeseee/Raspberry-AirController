#include "Am2302Sensor.hpp"
#include <Sensor/pi_2_dht_read.hpp>
#include <SysLogger.hpp>
#include <Utility/Constants.hpp>

#include <cassert>

namespace sensor
{

Am2302Sensor::Am2302Sensor(const gpio::IGpioPtr& sensor, const logger::SysLoggerPtr& sysLogger)
    : threading::Threading(CALL_INTERVALL_AM2302)
    , m_data(std::make_shared<SensorData>(SET_TEMP, SET_HUM))
    , m_sensor(sensor)
    , m_sysLogger(sysLogger)
    , m_loggerIdTemp(sysLogger->generateId("Indoor Temperature"))
    , m_loggerIdHum(sysLogger->generateId("Indoor Humidity"))

{
    assert(m_sensor);


    setInitialized();
}

SensorDataCPtr Am2302Sensor::getData() const
{
    return m_data;
}

void Am2302Sensor::recall()
{
    double humidity = 0.0;
    double temperature = 0.0;

    const DhtState dhtState = pi_2_dht_read(DHT22, m_sensor->getPinNumber(), &humidity, &temperature);

    if (DhtState::SUCCESS == dhtState) {
        m_data->m_temperature = temperature;
        m_data->m_humidity = humidity;
        m_sysLogger->logSensorValue(m_loggerIdTemp, m_data->m_temperature);
        m_sysLogger->logSensorValue(m_loggerIdHum, m_data->m_humidity);
    } else {
        const std::string msg = "1wire bus return invalid value of " + std::to_string(static_cast<size_t>(dhtState));
        m_sysLogger->logError(m_loggerIdTemp, msg);
        m_sysLogger->logError(m_loggerIdHum, msg);
    }
}
}
