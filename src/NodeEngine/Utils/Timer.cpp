#include "Timer.hpp"

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
