#include "WeatherStation.hpp"
#include <Constants.hpp>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <curl/curl.h>

#include <math.h>

namespace sensor {

static const float KELVIN = 273.15;
static const char *WEATHER_URL = "api.openweathermap.org/data/2.5/"
                                 "weather?id=7285765&APPID="
                                 "e018bcd525a923f820afd5b43cac259e";

WeatherStation::WeatherStation() : threading::Threading(CALL_INTERVALL_WEB) {}

void WeatherStation::recall() {
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, WEATHER_URL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    // curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);

    /* Check for errors */
    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
      return;
    }
  }

  updateData();
}

size_t writeCallback(char *buf, size_t size, size_t nmemb,
                     void *up) { // callback must have this declaration
  // buf is a pointer to the data that curl has for us
  // size*nmemb is the size of the buffer

  for (int c = 0; c < size * nmemb; c++) {
    receivedData.push_back(buf[c]);
  }
  return size * nmemb; // tell curl how many bytes we handled
}

SensorData WeatherStation::getData() const {
  return SensorData{m_temperature, m_humidity};
}

void WeatherStation::updateData() {
  std::stringstream ss;
  // send your JSON above to the parser below, but populate ss first
  ss << receivedData;
  receivedData = "";

  boost::property_tree::ptree pt;
  boost::property_tree::read_json(ss, pt);

  boost::optional<float> temp = pt.get_optional<float>("main.temp");
  // std::cout << "Outside temperature is " << temp.get() - KELVIN << std::endl;
  if (temp) {
    m_temperature = temp.get() - KELVIN;
  } else {
    m_temperature = std::numeric_limits<float>::min();
  }

  boost::optional<float> humidity = pt.get_optional<float>("main.humidity");
  // std::cout << "Outside humidity is " << humidity << std::endl;
  if (temp) {
    m_humidity = humidity.get();
  } else {
    m_humidity = std::numeric_limits<float>::min();
  }

  SensorData data = getData();
  std::cout << "Temperature: " << data.temperature
            << "\t Humidity: " << data.humidity << std::endl;
}
}
