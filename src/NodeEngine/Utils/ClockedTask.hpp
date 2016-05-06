#ifndef NCLOCKEDTASK_HPP
#define NCLOCKEDTASK_HPP

#include <functional>
#include <SFML/System/Clock.hpp>

class NClockedTask
{
    public:
        NClockedTask(std::function<void()> function = [](){});

        void setTask(std::function<void()> function);

        sf::Time execute();

    protected:
        std::function<void()> mFunction;
};

#endif // NCLOCKEDTASK_HPP
