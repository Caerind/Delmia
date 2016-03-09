#ifndef NTIMER_HPP
#define NTIMER_HPP

#include <functional>
#include <memory>
#include <SFML/System/Time.hpp>

class NTimer
{
    public:
        NTimer();
        ~NTimer();

        typedef std::shared_ptr<NTimer> Ptr;
        typedef std::function<void()> Callback;

        void setCallback(Callback callback);

        sf::Time getRemaining() const;
        sf::Time getDuration() const;

        void update(sf::Time dt);

        void reset(sf::Time duration);
        void stop();

    protected:
        Callback mCallback;

        bool mRunning;

        sf::Time mRemaining;
        sf::Time mDuration;
};

#endif // NTIMER_HPP
