#ifndef WEATHER_STATION_H
#define WEATHER_STATION_H

#include <memory>
#include <string>
#include <thread>

namespace weather_station {

struct WeatherData {
  float temperature; // [°C] temperature
  float humidity;    // [%] humadity
};

class WeatherStation {
public:
  WeatherStation();
  ~WeatherStation();

  void recall();

  WeatherData getData() const;

private:
  float relHumidityToAbs(float temperature, float humidity);
  void updateData();
  void threadFn();

  float m_temperature; // [°C]
  float m_humidity;    // [%]

  std::thread m_thread;
  bool m_stopThread{false};
};

static std::string receivedData; // will hold the url's contents

static size_t writeCallback(char *buf, size_t size, size_t nmemb, void *up);

} // weather_station
#endif
