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

NTimer::NTimer()
{
    mDuration = sf::Time::Zero;
    mRemaining = sf::Time::Zero;
    mElapsed = sf::Time::Zero;
    mRunning = false;
    mRepeat = false;
}

NTimer::~NTimer()
{
}

void NTimer::setCallback(NTimer::Callback callback)
{
    mCallback = callback;
}

sf::Time NTimer::getRemaining() const
{
    return mRemaining;
}

sf::Time NTimer::getElapsedTime() const
{
    return mElapsed;
}

sf::Time NTimer::getDuration() const
{
    return mDuration;
}

void NTimer::setRepeat(bool repeat)
{
    mRepeat = repeat;
}

bool NTimer::isRepeated() const
{
    return mRepeat;
}

bool NTimer::isRunning() const
{
    return mRunning;
}

void NTimer::update(sf::Time dt)
{
    if (mRunning)
    {
        mElapsed += dt;

        if (mDuration != sf::Time::Zero)
        {
            mRemaining -= dt;
            if (mRemaining <= sf::Time::Zero)
            {
                if (mCallback)
                {
                    mCallback();
                }

                if (mRepeat)
                {
                    reset(mDuration);
                }
                else
                {
                    stop();
                }
            }
        }
    }
}

void NTimer::play()
{
    mRunning = true;
}

void NTimer::pause()
{
    mRunning = false;
}

void NTimer::reset(sf::Time duration)
{
    mDuration = duration;
    mRemaining = duration;
    mElapsed = sf::Time::Zero;
    mRunning = true;
}

void NTimer::stop()
{
    mRunning = false;
    mRemaining = sf::Time::Zero;
    mDuration = sf::Time::Zero;
    mElapsed = sf::Time::Zero;
}

} // namespace NTime
