#ifndef NTIME_HPP
#define NTIME_HPP

#include <ctime>
#include <functional>
#include <memory>
#include <string>

#include <SFML/System/Time.hpp>

namespace NTime
{

std::string getTime(std::string const& timeFormat = "[%x][%X]");
double getTimeAsSeconds();

int getHours();
int getMinutes();
int getSeconds();

int getDay();
int getMonth();
int getYear();

} // namespace NTime

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

#endif // NTIME_HPP
