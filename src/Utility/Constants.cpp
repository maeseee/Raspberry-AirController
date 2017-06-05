#include "Constants.hpp"

#include <cmath>
#include <time.h>

float relHumidityToAbs(const float tempC, const float humidityRel)
{
    // function calculated for
    // Temperatur [°C]  water for 100 % [g/m3]
    // -20              0.9
    // -15	            1.4
    // -10	            2.1
    //  -5	            3.3
    //   0	            4.8
    //   5	            6.8
    //  10	            9.4
    //  15	           12.8
    //  20	           17.3
    //  25	           23.0
    //  30	           30.3
    //  35	           39.6
    //  40	           51.1

    return 4.2431796244 * exp(0.0666427637 * tempC) * humidityRel / 100;
}

float absHumidityToRel(const float tempC, const float humidityAbs)
{
    float absHumidity100 = relHumidityToAbs(tempC, 100);
    return humidityAbs / absHumidity100 * 100;
}

bool shouldWarm()
{
    time_t t = time(NULL); // get time now
    struct tm* now = localtime(&t);

    // Example of date of today
    //  std::cout << "Date of today is: " << (now->tm_year + 1900) << '-'
    //            << (now->tm_mon + 1) << '-' << now->tm_mday << std::endl;

    const size_t month = now->tm_mon + 1;
    const size_t day = now->tm_mday;
    const float month_fp = static_cast<float>(month) + static_cast<float>(day) / 30.0;
    if ((month_fp > 5.5) && (month_fp < 9.0)) {
        // Cool in the months July and August
        return false;
    } else {
        return true;
    }
}

size_t getDaytime()
{
    time_t t = time(0); // get time now
    struct tm* now = localtime(&t);
    const size_t daytime = (now->tm_hour * HOUR_TO_SEC) + (now->tm_min * MIN_TO_SEC) + now->tm_sec;
    return daytime;
}
