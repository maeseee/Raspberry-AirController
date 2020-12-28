#include "Constants.hpp"

#include <ctime>

size_t getDaytime()
{
    time_t t = time(nullptr); // get time now
    struct tm* now = localtime(&t);
    const size_t daytime = (now->tm_hour * HOUR_TO_SEC) + (now->tm_min * MIN_TO_SEC) + now->tm_sec;
    return daytime;
}
