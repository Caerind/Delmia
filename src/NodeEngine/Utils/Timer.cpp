#include "Timer.hpp"

NTimer::NTimer()
{
    mDuration = sf::Time::Zero;
    mRemaining = sf::Time::Zero;
    mRunning = false;
}

NTimer::~NTimer()
{
    mDuration = sf::Time::Zero;
    mRemaining = sf::Time::Zero;
    mRunning = false;
}

void NTimer::setCallback(NTimer::Callback callback)
{
    mCallback = callback;
}

sf::Time NTimer::getRemaining() const
{
    return mRemaining;
}

sf::Time NTimer::getDuration() const
{
    return mDuration;
}

void NTimer::update(sf::Time dt)
{
    if (mRunning)
    {
        mRemaining -= dt;
        if (mRemaining <= sf::Time::Zero)
        {
            if (mCallback)
            {
                mCallback();
            }
            mRunning = false;
            mRemaining = sf::Time::Zero;
            mDuration = sf::Time::Zero;
        }
    }
}

void NTimer::reset(sf::Time duration)
{
    mDuration = duration;
    mRemaining = duration;
    mRunning = true;
}

void NTimer::stop()
{
    mRunning = false;
    mRemaining = sf::Time::Zero;
    mDuration = sf::Time::Zero;
}
