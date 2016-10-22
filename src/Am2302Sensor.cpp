#include <Am2302Sensor.hpp>
#include <Constants.hpp>
#include <pi_2_dht_read.hpp>

#include <iostream>

namespace sensor {

Am2302Sensor::Am2302Sensor(const gpio::IGpioPtr &sensor)
    : threading::Threading(CALL_INTERVALL_AM2302), m_sensor(sensor) {}

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
  } else {
    std::cout << "1wire bus return invalid value of " << returnValue
              << std::endl;
  }
  std::cout << "Sensorvalue measured of humidity=" << m_humidity
            << "% and temperature=" << m_temperature << "°C" << std::endl;
}
}
