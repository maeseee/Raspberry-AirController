#include "WeatherStation.hpp"
#include <SysLogger.hpp>
#include <Utility/Constants.hpp>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <curl/curl.h>

namespace sensor
{

static const float KELVIN = 273.15;
static const char* CURRENT_WEATHER_URL =
    "api.openweathermap.org/data/2.5/"
    "weather?id=7285765&APPID="
    "e018bcd525a923f820afd5b43cac259e";

// static const char* FORECAST_WEATHER_URL =
//    "api.openweathermap.org/data/2.5/"
//    "forecast?id=7285765&APPID="
//    "e018bcd525a923f820afd5b43cac259e";

WeatherStation::WeatherStation(const logger::SysLoggerPtr& sysLogger)
    : threading::Threading(CALL_INTERVALL_WEB)
    , m_temperature(INVALID_FLOAT)
    , m_humidity(INVALID_FLOAT)
    , m_sysLogger(sysLogger)
    , m_loggerIdTemp(m_sysLogger->generateId("Outdoor Temperature"))
    , m_loggerIdHum(m_sysLogger->generateId("Outdoor Humidity"))
    , m_loggerIdCon(m_sysLogger->generateId("Outdoor Connection"))
{

    setInitialized();
}

void WeatherStation::recall()
{
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, CURRENT_WEATHER_URL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        // curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* always cleanup */
        curl_easy_cleanup(curl);

        /* Check for errors */
        if (res != CURLE_OK) {
            m_sysLogger->logError(m_loggerIdCon, "curl_easy_perform() failed: " + std::string(curl_easy_strerror(res)));
            return;
        }
    }

    updateData();
}

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* /*up*/)
{ // callback must have this declaration
    // buf is a pointer to the data that curl has for us
    // size*nmemb is the size of the buffer

    for (size_t c = 0; c < size * nmemb; c++) {
        m_receivedData.push_back(buf[c]);
    }
    return size * nmemb; // tell curl how many bytes we handled
}

SensorDataPtr WeatherStation::getData() const
{
    if (INVALID_FLOAT >= m_temperature) {
        m_sysLogger->logError(m_loggerIdTemp, "Invalid outdoor value");
        return nullptr;
    } else if (INVALID_FLOAT >= m_humidity) {
        m_sysLogger->logError(m_loggerIdHum, "Invalid outdoor value");
        return nullptr;
    } else {
        return std::make_shared<SensorData>(m_temperature, m_humidity);
    }
}

void WeatherStation::updateData()
{
    std::stringstream ss;
    // send your JSON above to the parser below, but populate ss first
    ss << m_receivedData;
    m_receivedData = "";

    boost::property_tree::ptree pt;
    boost::property_tree::read_json(ss, pt);

    boost::optional<float> temp = pt.get_optional<float>("main.temp");

    if (temp) {
        m_temperature = temp.get() - KELVIN;
        m_sysLogger->logSensorValue(m_loggerIdTemp, m_temperature);
    } else {
        m_temperature = INVALID_FLOAT;
        m_sysLogger->logError(m_loggerIdTemp, "invalid new outdoor temp");
    }

    boost::optional<float> humidity = pt.get_optional<float>("main.humidity");
    if (temp) {
        m_humidity = humidity.get();
        m_sysLogger->logSensorValue(m_loggerIdHum, m_humidity);
    } else {
        m_humidity = INVALID_FLOAT;
        m_sysLogger->logError(m_loggerIdHum, "invalid new outdoor hum");
    }
}
}
