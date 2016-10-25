#pragma once

#include <Sensor/ISensor.hpp>
#include <Threading.hpp>

#include <memory>
#include <string>

namespace sensor {

class WeatherStation : public sensor::ISensor, public threading::Threading {
public:
  WeatherStation();

  SensorData getData() const override;

  void recall() override;

private:
  void updateData();

  float m_temperature; // [°C]
  float m_humidity;    // [%]
};

static std::string m_receivedData; // will hold the url's contents

static size_t writeCallback(char *buf, size_t size, size_t nmemb, void *up);
}
