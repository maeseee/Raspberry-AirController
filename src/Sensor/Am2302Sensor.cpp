#include <Constants.hpp>
#include <Sensor/Am2302Sensor.hpp>
#include <Sensor/pi_2_dht_read.hpp>
#include <SysLogger.hpp>

namespace sensor {

Am2302Sensor::Am2302Sensor(const gpio::IGpioPtr &sensor,
                           const logger::SysLoggerPtr &sysLogger)
    : threading::Threading(CALL_INTERVALL_AM2302), m_sensor(sensor),
      m_sysLogger(sysLogger) {
  m_loggerIdTemp = m_sysLogger->generateId("Indoor Temperature");
  m_loggerIdHum = m_sysLogger->generateId("Indoor Humidity");
}

SensorData Am2302Sensor::getData() const {
  return SensorData{m_temperature, m_humidity};
}

void Am2302Sensor::recall() {
  float humidity = 0;
  float temperature = 0;

  int returnValue =
      pi_2_dht_read(DHT22, m_sensor->getPinNumber(), &humidity, &temperature);

  if (DHT_SUCCESS == returnValue) {
    m_humidity = humidity;
    m_temperature = temperature;
    m_sysLogger->logSensorValue(m_loggerIdTemp, m_temperature);
    m_sysLogger->logSensorValue(m_loggerIdHum, m_humidity);
  } else {
    m_humidity = std::numeric_limits<float>::min();
    m_temperature = std::numeric_limits<float>::min();
    m_sysLogger->logError(m_loggerIdTemp, "1wire bus return invalid value of " +
                                              std::to_string(returnValue));
  }
}
}
