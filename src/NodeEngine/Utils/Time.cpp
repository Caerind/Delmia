#include "Time.hpp"

namespace NTime
{

std::string getTime(std::string const& timeFormat)
{
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,80,timeFormat.c_str(),timeinfo);
    return std::string(buffer);
}

double getTimeAsSeconds()
{
    time_t timer;
    struct tm y2k15 = {0};
    float seconds;

    y2k15.tm_hour = 0;   y2k15.tm_min = 0; y2k15.tm_sec = 0;
    y2k15.tm_year = 116; y2k15.tm_mon = 1; y2k15.tm_mday = 1; // 1/02/2016

    time(&timer);  /* get current time; same as: timer = time(NULL)  */

    seconds = difftime(timer,mktime(&y2k15));

    return seconds;
}

int getHours()
{
    return std::stoi(getTime("%H"));
}

int getMinutes()
{
    return std::stoi(getTime("%M"));
}

int getSeconds()
{
    return std::stoi(getTime("%S"));
}

int getDay()
{
    return std::stoi(getTime("%d"));
}

int getMonth()
{
    return std::stoi(getTime("%m"));
}

int getYear()
{
    return std::stoi(getTime("%y"));
}

} // namespace NTime
