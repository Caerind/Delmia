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
        sf::Time getElapsedTime() const;
        sf::Time getDuration() const;

        void setRepeat(bool repeat);
        bool isRepeated() const;

        bool isRunning() const;

        void update(sf::Time dt);

        void play();
        void pause();
        void reset(sf::Time duration);
        void stop();

    protected:
        Callback mCallback;

        bool mRunning;
        bool mRepeat;

        sf::Time mRemaining;
        sf::Time mElapsed;
        sf::Time mDuration;
};

#endif // NTIMER_HPP
