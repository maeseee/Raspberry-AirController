#include "WeatherStation.hpp"

#include <boost/program_options.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <curl/curl.h>

#include <functional>
#include <limits>
#include <math.h>

namespace weather_station {

static constexpr float KELVIN = 273.15;
static const char *WEATHER_URL = "api.openweathermap.org/data/2.5/"
                                 "weather?id=7285765&APPID="
                                 "e018bcd525a923f820afd5b43cac259e";
static const size_t CALL_INTERVALL =
    10 * 60; // call upcate intervall for thread

WeatherStation::WeatherStation() {
  m_thread = std::thread(&WeatherStation::threadFn, this);
}

WeatherStation::~WeatherStation() {
  m_stopThread = true;
  m_thread.join();
}

void WeatherStation::recall() {
  std::cout << "---recall" << std::endl;
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

WeatherData WeatherStation::getData() const {
  return WeatherData{m_temperature, m_humidity};
}

float WeatherStation::relHumidityToAbs(float temperature, float humidity) {
  // function calculated for
  // Temperatur [°C]  water for 100 % [g/m3]
  // -20              0.9
  // -15	          1.4
  // -10	          2.1
  // -5	              3.3
  // 0	              4.8
  // 5	              6.8
  // 10	              9.4
  // 15	              12.8
  // 20	              17.3
  // 25	              23
  // 30	              30.3
  // 35	              39.6
  // 40	              51.1

    float absHumidity = pow(4.2431796244, 0.0666427639  * temperature) * humidity / 100;
}

void WeatherStation::updateData() {
  std::cout << "---updateData" << std::endl;
  std::cout << receivedData << std::endl;
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
}

void WeatherStation::threadFn() {
  while (!m_stopThread) {
    static int timeCounter = 0;
    if (0 == timeCounter) {
      recall();
    }
    ++timeCounter;
    timeCounter %= 600;
    sleep(1);
  }
}
}
