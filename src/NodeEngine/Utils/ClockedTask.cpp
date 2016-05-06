#include "ClockedTask.hpp"

NClockedTask::NClockedTask(std::function<void()> function)
: mFunction(function)
{
}

void NClockedTask::setTask(std::function<void()> function)
{
    mFunction = function;
}

sf::Time NClockedTask::execute()
{
    if (mFunction)
    {
        sf::Clock c;
        mFunction();
        return c.getElapsedTime();
    }
    else
    {
        return sf::Time::Zero;
    }
}
