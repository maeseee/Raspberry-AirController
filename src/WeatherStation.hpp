#ifndef WEATHER_STATION_H
#define WEATHER_STATION_H

#include <ISensor.hpp>

#include <memory>
#include <string>
#include <thread>

namespace sensor {

class WeatherStation : public sensor::ISensor {
public:
  WeatherStation();
  ~WeatherStation();

  SensorData getData() const override;

private:
  void updateData();

  void recall();
  void threadFn();

  float m_temperature; // [°C]
  float m_humidity;    // [%]

  std::thread m_thread;
  bool m_stopThread{false};
};

static std::string receivedData; // will hold the url's contents

static size_t writeCallback(char *buf, size_t size, size_t nmemb, void *up);

} // sensor
#endif
